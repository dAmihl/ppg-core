#include "WfcGenerator.h"

namespace PPG
{
	UPtr<Puzzle> WfcGenerator::generatePuzzle(Context context)
	{
		Vec<Ptr<Object>>& objects = context.getObjects();
		Vec<Ptr<Event>>& events = context.getEvents();
		Vec<Rule>& rules = context.getRules();

		Logger::log("Start generating a new Puzzle.");
		Logger::log("Using rules:");
		for (auto& it : rules) {
			Logger::log(it.getTextualRepresentation());
		}

		// initialize randomizer
		if (seedSet) {
			Randomizer::init(seed);
		}
		else {
			Randomizer::init();
		}

		UPtr<Puzzle> P = makeU<Puzzle>();

		/* Generate Nodes and add to puzzle P*/
		if (numberNodes == 0) numberNodes = objects.size();


		/* Generate Relation and add to Puzzle P */
		Relation R;
		P->setNodes(generateNodes(objects, numberNodes));
		P->setRelation(generateRelation(P, P->getNodes(), rules));

		cleanupNodes(P);
		initializeActivePropertyOnNodes(P);
		P->setContext(context);

		return P;
	}

	NodeVec WfcGenerator::generateUniqueNodes(const ObjVec& objects, size_t numNodes)
	{
		NodeVec nodes;
		for (auto& o : objects)
		{
			for (auto& s : o->getReachableStates())
			{
				nodes.emplace_back(make<Node>(o, s));
			}
		}
		return nodes;
	}

	NodeVec WfcGenerator::generateNodes(const ObjVec& objects, size_t numNodes)
	{
		NodeVec nodes;

		for (size_t i = 0; i < numNodes; i++) {
			Ptr<Object> obj = Randomizer::getRandomFromList(objects);
			if (obj == nullptr) continue;

			Vec<State> reachableStates = obj->getReachableStates();
			if (reachableStates.empty()) continue;

			State state = Randomizer::getRandomFromList(reachableStates);
			Ptr<Node> newNode = make<Node>(obj, state);
			nodes.push_back(newNode);
		}
		return nodes;
	}


	Vec<size_t> findIndicesByPattern(const NodeVec& nodes, std::function<bool(const Ptr<Node> n)> pred)
	{
		Vec<size_t> res;
		for (size_t i = 0; i < nodes.size(); ++i)
		{
			if (pred(nodes.at(i)))
			{
				res.push_back(i);
			}
		}
		return res;
	}

	bool getRandomAvailableIndex(size_t& indxI, size_t& indxJ, const WfcMat& mat, const size_t numNodes)
	{
		Vec<Pair<size_t, size_t>> indices;
		for (size_t i = 0; i < numNodes; ++i)
		{
			for (size_t j = 0; j < numNodes; ++j)
			{
				if (mat.at(i,j) == EWfcCellState::FREE)
				{
					indices.emplace_back(i, j);
				}
			}
		}

		if (indices.empty()) return false;

		auto randPair = indices[Randomizer::getRandomUintFromRange(0, indices.size())];

		indxI = randPair.first;
		indxJ = randPair.second;
		return true;
	}

	bool getNextAvailableIndex(size_t& indxI, size_t& indxJ, const WfcMat& mat, const size_t numNodes)
	{
		// count number of possible connections (entropy)
		// for each index
		size_t numAvailableI = 0;
		size_t numAvailableJ = 0;

		Vec<Pair<size_t, size_t>> indicesI;
		Vec<Pair<size_t, size_t>> indicesJ;
		Vec<Pair<size_t, size_t>> indices;

		for (size_t i = 0; i < numNodes; ++i)
		{
			for (size_t j = 0; j < numNodes; ++j)
			{
				if (mat.at(i, j) == EWfcCellState::FREE)
				{
					if (i == indxI || j == indxI)
					{
						numAvailableI++;
						indicesI.emplace_back(i, j);
					}

					if (i == indxJ || j == indxJ) {
						numAvailableJ++;
						indicesJ.emplace_back(i, j);
					}

					indices.emplace_back(i, j);
				}
			}
		}

		if (indices.empty()) return false;

		Pair<size_t, size_t> randpair;

		if (numAvailableI > 0 && numAvailableI < numAvailableJ)
		{
			randpair = indicesI[Randomizer::getRandomUintFromRange(0, indicesI.size())];
		}
		else if (numAvailableJ > 0 && numAvailableJ < numAvailableI)
		{
			randpair = indicesJ[Randomizer::getRandomUintFromRange(0, indicesJ.size())];
		}
		else
		{
			randpair = indices[Randomizer::getRandomUintFromRange(0, indices.size())];
		}
		

		indxI = randpair.first;
		indxJ = randpair.second;
		return true;
	}



	void initMatrixState(NodeVec& nodes, WfcMat& mat, RuleVec& rules)
	{

		// set identities to false, rest default to true
		for (size_t i = 0; i < nodes.size(); ++i)
		{
			for (size_t j = 0; j < nodes.size(); ++j)
			{
				if (i == j)
				{
					mat.set(i, j, EWfcCellState::NOT);
				}
			}
		}

		// for each rule, set related flags to false
		for (auto& rule : rules)
		{
			auto& lhsO = rule.getLeftHandSideObject();
			auto& rhsO = rule.getRightHandSideObject();
			auto lhsS = rule.getLeftHandSideState();
			auto rhsS = rule.getRightHandSideState();

			NodeVec lhsNs;
			NodeVec rhsNs;

			// find the nodes which are relevant for the rules
			auto predLhs = [obj = lhsO, st = lhsS](const Ptr<Node> n) {
				return n->getRelatedObject() == obj && (st == STATE_ANY || n->getGoalState() == st);
			};

			auto predRhs = [obj = rhsO, st = rhsS](const Ptr<Node> n) {
				return n->getRelatedObject() == obj && (st == STATE_ANY || n->getGoalState() == st);
			};

			Vec<size_t> lhsIndices = findIndicesByPattern(nodes, predLhs);
			Vec<size_t> rhsIndices = findIndicesByPattern(nodes, predRhs);

			// for rule type before. we can set every direct combination where RHS < LHS to false
			// for rule type after, we can set every direct combination where LHS < RHS to false
			// for rule type strict before A <! B, we can disallow every other Node W such that A < W
			// for rule type strict after A >! B, we can disallow every other node W such that A > W
			switch (rule.getRuleType())
			{
			case Rule::EPuzzleRuleType::AFTER:
			{
				for (size_t i : lhsIndices)
				{
					for (size_t j : rhsIndices)
					{
						mat.set(i, j, EWfcCellState::NOT);
					}
				}
				break;
			}

			case Rule::EPuzzleRuleType::BEFORE:
			{
				for (size_t i : rhsIndices)
				{
					for (size_t j : lhsIndices)
					{
						mat.set(i, j, EWfcCellState::NOT);
					}
				}
				break;
			}

			case Rule::EPuzzleRuleType::STRICT_AFTER:
			{
				// X >! Y means that X -> Y NOT
				// and for every W != Y, W -> X is NT-NOT (non transitive)
				
				// for every X node
				for (size_t i : lhsIndices)
				{
					// and column
					for (size_t j = 0; j < nodes.size(); ++j)
					{
						// if node is Y node
						bool exists = std::find(rhsIndices.begin(), rhsIndices.end(), j) != std::end(rhsIndices);
						if (exists)
						{
							// set X -> Y to NOT
							mat.set(i, j, EWfcCellState::NOT);
						}
						else
						{
							// set W -> X NT-NOT
							mat.set(j, i, EWfcCellState::NTNOT);
						}
					}
				}

				break;
			}

			case Rule::EPuzzleRuleType::STRICT_BEFORE:
			{
				// X <! Y means that Y -> X is NOT
				// and for every W != X, X -> W is NT NOT

				// for every X node
				for (size_t i : lhsIndices)
				{
					// and column
					for (size_t j = 0; j < nodes.size(); ++j)
					{
						// if node is Y node
						bool exists = std::find(rhsIndices.begin(), rhsIndices.end(), j) != std::end(rhsIndices);
						if (exists)
						{
							// set Y -> X to NOT
							mat.set(j, i, EWfcCellState::NOT);
						}
						else
						{
							// set X -> W NT-NOT
							mat.set(i, j, EWfcCellState::NTNOT);
						}
					}
				}
				break;
			}

			}

		}
	}


	void collapseNode(size_t x, size_t y, WfcMat& mat, NodeVec& nodes, Relation& rel, RuleVec& rules)
	{
		mat.set(x, y, EWfcCellState::USED);
		// also disallow the inverse
		mat.set(y, x, EWfcCellState::NOT);

		auto& nx = nodes[x];
		auto& ny = nodes[y];
		// add node X -> Y
		rel.addPair(makePair(nx, ny));

		// take NOTs from row Y and copy to row X where cell is available
		for (size_t k = 0; k < nodes.size(); ++k)
		{
			if (mat.at(k, y) == EWfcCellState::NOT)
			{
				if (mat.at(k, x) == EWfcCellState::FREE || mat.at(k, x) == EWfcCellState::NTNOT)
				{
					mat.set(k, x, EWfcCellState::NOT);
				}
				
			}
		}

		// take NOTs from col X (X -> *) and copy to col Y where cell is available
		// "every node where X is not allowed to connect, Y is now also not allowed to connect"
		for (size_t k = 0; k < nodes.size(); ++k)
		{
			if (mat.at(x, k) == EWfcCellState::NOT)
			{
				if (mat.at(y, k) == EWfcCellState::FREE || mat.at(y, k) == EWfcCellState::NTNOT)
				mat.set(y, k, EWfcCellState::NOT);
			}
		}
	}


	
	PPG::Relation WfcGenerator::generateRelation(UPtr<Puzzle>& P, NodeVec& nodes, RuleVec& rules)
	{
		Relation rel;

		// Initialize Array. index 1 is node from, index 2 is node 
		WfcMat mat{ nodes.size() };

		initMatrixState(nodes, mat, rules);

		// use resulting array until every node is unavailable
		size_t x;
		size_t y;

		// choose wave origin randomly
		getRandomAvailableIndex(x, y, mat, nodes.size());
		collapseNode(x, y, mat, nodes, rel, rules);

		// then 
		while (getNextAvailableIndex(x, y, mat, nodes.size()))
		{
			// collapse (x,y)
			collapseNode(x, y, mat, nodes, rel, rules);
		}


		return rel;
	}

	/*
	*	Initialize all ACTIVE nodes with state ACTIVE
	*	Remember: Only active nodes are initially able to handle events
	*/
	void WfcGenerator::initializeActivePropertyOnNodes(UPtr<Puzzle>& P)
	{
		NodeVec nodes = P->getNodes();

		for (auto& it : nodes) {
			if (!P->getRelation().hasPrecedingNode(it)) {
				it->setPuzzleNodeState(ENodeState::ACTIVE);
			}
		}
	}

	void WfcGenerator::removeNodeFromList(const Ptr<Node>& N, NodeVec& nodes) {
		auto found = std::find(nodes.begin(), nodes.end(), N);
		if (found != nodes.end()) {
			nodes.erase(found);
		}
	}

	/**
	*	In order to don't have two meta-equal nodes O1 and O2, where O1 has dependencies and O2 does NOT,
	*	this method will perform a cleanup of the nodes after the relation is generated.
	*
	*/
	void WfcGenerator::cleanupNodes(UPtr<Puzzle>& P) {
		NodeVec& nodes = P->getNodes();
		NodeVec nodesToDelete;
		const Relation& R = P->getRelation();

		for (auto& it : nodes) {
			if (!R.hasFollowingNode(it) && !R.hasPrecedingNode(it)) {
				for (auto& find : nodes) {
					if (find != it && GeneratorHelper::checkEquality(it, find)) {
						nodesToDelete.push_back(it);
					}
				}
			}
		}

		for (auto& it : nodesToDelete) {
			removeNodeFromList(it, nodes);
		}
	}
}
