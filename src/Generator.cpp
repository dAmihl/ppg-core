#include "Generator.h"

namespace PPG {

	UPtr<Puzzle> Generator::generatePuzzle(Context context)
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
		P->setNodes(generateNodes(objects, numberNodes));

		/* Generate Relation and add to Puzzle P */
		Relation R;
		switch (genType)
		{
		case EGenType::STANDARD:
		case EGenType::EXPERIMENTAL:
		{
			P->setRelation(generateRelationExperimental(P, P->getNodes(), rules));
			break;
		}
		case EGenType::WFC:
		{
			P->setRelation(generateRelationWFC(P, P->getNodes(), rules));
			break;
		}
		}
		
		cleanupNodes(P);
		initializeActivePropertyOnNodes(P);
		P->setContext(context);

		return P;
	}

	PPG::UPtr<PPG::Puzzle> Generator::generatePuzzle(Context context, EGenType type)
	{
		genType = type;
		return generatePuzzle(context);
	}

	void Generator::removeNodeFromList(const Ptr<Node>& N, NodeVec& nodes) {
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

	void Generator::cleanupNodes(UPtr<Puzzle>& P) {
		NodeVec& nodes = P->getNodes();
		NodeVec nodesToDelete;
		const Relation& R = P->getRelation();

		for (auto& it: nodes) {
			if (!R.hasFollowingNode(it) && !R.hasPrecedingNode(it)) {
				for (auto& find: nodes) {
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


	/*
	*	## MAIN GENERATION METHOD ##
	*	Generating a relation of given nodes;
	*	by keeping invariants and thus solvability within this system
	*/
	PPG::Relation Generator::generateRelation(NodeVec& nodes, RuleVec rules)
	{
		Relation rel;

		/*
		* For Reference  a simple sequential generation of a relation
		for (std::vector<PuzzleNode*>::iterator it = nodes.begin(); it != nodes.end() - 1; ++it) {
			PuzzleNode* N1 = *it;
			PuzzleNode* N2 = *(it + 1);
			rel->addPair(N1, N2);
		}*/

		/*
		*	In order for a puzzle to be solvable within this system, the relation has to fulfill invariants.
		*	In this implementation,
		*	Check for:
		*	 - Circular Dependency,
		*	 - Parallel Object Equality,
		*	 - Direct Sequential Equality in Object and State,
		*	 - Exclusive Dependency
		*
		*	Also, custom Rules will be checked afterwards.
		*	The resulting node for the pair is a random node of a list of compatible nodes, which
		*	fulfill the above mentioned.
		*/

		for (auto& it: nodes) {
			/*
			* checks for basic rules (exclusive dependency, equality, circular dependency etc) and for custom rules
			*/
			NodeVec compList = GeneratorHelper::filterCompatibleNodes(it, rel, nodes, rules);

			// returns nullptr if list is empty
			Ptr<Node> N2 = Randomizer::getRandomFromList(compList);

			if (N2 != nullptr) {
				rel.addPair(it, N2);
			}
		}

		return rel;
	}

	PPG::Relation Generator::generateRelationExperimental(UPtr<Puzzle>& P, NodeVec& nodes, RuleVec& rules)
	{
		Relation rel;

		NodeVec nodesInGraph;

		Ptr<Node> NStart = Randomizer::getRandomFromList(nodes);
		nodesInGraph.push_back(NStart);

		for (auto& it: nodes) {

			// Copies nodesInGraph
			// Add N1 temporary for filterCompatibleNodes
			NodeVec tmpNodes = nodesInGraph;
			tmpNodes.push_back(it);

			/*
			* checks for basic rules (exclusive dependency, equality, circular dependency etc) and for custom rules
			*/
			NodeVec compList = GeneratorHelper::filterCompatibleNodes(it, rel, tmpNodes, rules);

			// returns nullptr if list is empty
			Ptr<Node> N2 = Randomizer::getRandomFromList(compList);

			if (N2 != nullptr) {
				rel.addPair(it, N2);
				nodesInGraph.push_back(it);
			}
		}

		P->setNodes(nodesInGraph);

		return rel;
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


	PPG::Relation Generator::generateRelationWFC(UPtr<Puzzle>& P, const NodeVec& nodes, RuleVec& rules)
	{

		Relation rel;

		// Initialize Array. index 1 is node from, index 2 is node to
		Vec<bool> arr(nodes.size()*nodes.size(), true);

		// set identities to false, rest default to true
		for (size_t i = 0; i < nodes.size(); ++i)
		{
			for (size_t j = 0; j < nodes.size(); ++j)
			{
				if (i == j)
				{
					arr[i*nodes.size() + j] = false;
				}
			}
		}

		// for each rule, set flags to false
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
			switch (rule.getRuleType())
			{
			case Rule::EPuzzleRuleType::AFTER:
			case Rule::EPuzzleRuleType::STRICT_AFTER:
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
			case Rule::EPuzzleRuleType::STRICT_BEFORE:
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
			
			}

		}
		return rel;
	}

	NodeVec Generator::generateNodes(const ObjVec& objects, size_t numNodes)
	{
		NodeVec nodes;

		for (size_t i = 0; i < numNodes; i++) {
			Ptr<Object> obj = Randomizer::getRandomFromList(objects);
			if (obj == nullptr) continue;
			try {
				State state = Randomizer::getRandomFromList(obj->getReachableStates());
				Ptr<Node> newNode = make<Node>(obj, state);
				nodes.push_back(newNode);
			}
			catch (int error) {
				// TODO error handling
				(void)error;
			}
		}
		return nodes;
	}


	void Generator::setSeed(unsigned int seed)
	{
		seed = seed;
		seedSet = true;
	}

	unsigned int Generator::getSeed() const
	{
		return seed;
	}

	/*
	*	Initialize all ACTIVE nodes with state ACTIVE
	*	Remember: Only active nodes are initially able to handle events
	*/
	void Generator::initializeActivePropertyOnNodes(UPtr<Puzzle>& P)
	{
		NodeVec nodes = P->getNodes();

		for (auto& it: nodes) {
			if (!P->getRelation().hasPrecedingNode(it)) {
				it->setPuzzleNodeState(ENodeState::ACTIVE);
			}
		}
	}

}
