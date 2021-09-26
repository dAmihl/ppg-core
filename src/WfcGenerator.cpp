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
		P->setNodes(generateUniqueNodes(objects, numberNodes));
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

	bool getRandomAvailableIndex(size_t& indxI, size_t& indxJ, const Vec<bool>& arr, const size_t numNodes)
	{
		Vec<Pair<size_t, size_t>> indices;
		for (size_t i = 0; i < numNodes; ++i)
		{
			for (size_t j = 0; j < numNodes; ++j)
			{
				if (arr[i * numNodes + j])
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



	void initMatrixState(NodeVec& nodes, Vec<bool>& arr, RuleVec& rules)
	{

		// set identities to false, rest default to true
		for (size_t i = 0; i < nodes.size(); ++i)
		{
			for (size_t j = 0; j < nodes.size(); ++j)
			{
				if (i == j)
				{
					arr[i * nodes.size() + j] = false;
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
						arr[i * nodes.size() + j] = false;
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
						arr[i * nodes.size() + j] = false;
					}
				}
				break;
			}

			case Rule::EPuzzleRuleType::STRICT_AFTER:
			{
				// for every RHS node
				for (size_t i : rhsIndices)
				{
					// set all other nodes to false except the ones on LHS
					for (size_t j = 0; j < nodes.size(); ++j)
					{
						arr[i * nodes.size() + j] = std::find(lhsIndices.begin(), lhsIndices.end(), j) != std::end(lhsIndices);
					}
				}
				break;
			}

			case Rule::EPuzzleRuleType::STRICT_BEFORE:
			{
				// for every LHS node
				for (size_t i : lhsIndices)
				{
					// set all other nodes to false except the ones on RHS
					for (size_t j = 0; j < nodes.size(); ++j)
					{
						arr[i * nodes.size() + j] = std::find(rhsIndices.begin(), rhsIndices.end(), j) != std::end(rhsIndices);
					}
				}
				break;
			}

			}

		}
	}


	void collapseNode(size_t x, size_t y, Vec<bool>& arr, NodeVec& nodes, Relation& rel, RuleVec& rules)
	{
		arr[x * nodes.size() + y] = false;
		// also disallow the inverse
		arr[y * nodes.size() + x] = false;

		auto& nx = nodes[x];
		auto& ny = nodes[y];
		// add node X -> Y
		rel.addPair(makePair(nx, ny));

		// now check every rule for X and Y
		// e.g. Y < W for a node W
		// disallow node W -> X
		// also for a rule X > W disallow Y -> W node

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

			// Check Rule type
			switch (rule.getRuleType())
			{
			case Rule::EPuzzleRuleType::BEFORE:
			case Rule::EPuzzleRuleType::STRICT_BEFORE:
			{
				// a rule Y < W
				if (predLhs(ny))
				{
					auto predX = [obj = nx->getRelatedObject(), st = nx->getGoalState()](const Ptr<Node> n) {
						return n->getRelatedObject() == obj && (st == STATE_ANY || n->getGoalState() == st);
					};

					// disallow every node W -> X
					Vec<size_t> wIndices = findIndicesByPattern(nodes, predRhs);
					Vec<size_t> xIndices = findIndicesByPattern(nodes, predX);

					for (size_t i : wIndices)
					{
						for (size_t j : xIndices)
						{
							arr[i * nodes.size() + j] = false;
						}
					}
				}
				break;
			}

			case Rule::EPuzzleRuleType::AFTER:
			case Rule::EPuzzleRuleType::STRICT_AFTER:
			{
				// a rule X > W
				if (predLhs(nx))
				{
					auto predY = [obj = ny->getRelatedObject(), st = ny->getGoalState()](const Ptr<Node> n) {
						return n->getRelatedObject() == obj && (st == STATE_ANY || n->getGoalState() == st);
					};

					// disallow Y -> W node
					Vec<size_t> wIndices = findIndicesByPattern(nodes, predRhs);
					Vec<size_t> yIndices = findIndicesByPattern(nodes, predY);

					for (size_t i : yIndices)
					{
						for (size_t j : wIndices)
						{
							arr[i * nodes.size() + j] = false;
						}
					}
				}
				break;
			}
			}

		}
	}


	
	PPG::Relation WfcGenerator::generateRelation(UPtr<Puzzle>& P, NodeVec& nodes, RuleVec& rules)
	{
		Relation rel;

		// Initialize Array. index 1 is node from, index 2 is node to
		Vec<bool> arr(nodes.size() * nodes.size(), true);

		initMatrixState(nodes, arr, rules);

		// use resulting array until every node is unavailable
		size_t x;
		size_t y;
		while (getRandomAvailableIndex(x, y, arr, nodes.size()))
		{
			// collapse (x,y)
			collapseNode(x, y, arr, nodes, rel, rules);
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
