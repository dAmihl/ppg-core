#include "DefaultGenerator.h"

namespace PPG {

	UPtr<Puzzle> DefaultGenerator::generatePuzzle(Context context)
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


	void DefaultGenerator::removeNodeFromList(const Ptr<Node>& N, NodeVec& nodes) {
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
	void DefaultGenerator::cleanupNodes(UPtr<Puzzle>& P) {
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
	PPG::Relation DefaultGenerator::generateRelationSimple(NodeVec& nodes, RuleVec rules)
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

	PPG::Relation DefaultGenerator::generateRelation(UPtr<Puzzle>& P, NodeVec& nodes, RuleVec& rules)
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

	

	NodeVec DefaultGenerator::generateNodes(const ObjVec& objects, size_t numNodes)
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


	/*
	*	Initialize all ACTIVE nodes with state ACTIVE
	*	Remember: Only active nodes are initially able to handle events
	*/
	void DefaultGenerator::initializeActivePropertyOnNodes(UPtr<Puzzle>& P)
	{
		NodeVec nodes = P->getNodes();

		for (auto& it: nodes) {
			if (!P->getRelation().hasPrecedingNode(it)) {
				it->setPuzzleNodeState(ENodeState::ACTIVE);
			}
		}
	}

}
