#include "Generator.h"

namespace PPG {

	Puzzle* Generator::generatePuzzle(ObjVec& objects, EventVec& events, RuleVec& rules)
	{
		Logger::log("Start generating a new Puzzle.");
		Logger::log("Using rules:");
		for (auto& it : rules) {
			Logger::log(it.getTextualRepresentation());
		}

		// initialize randomizer
		if (this->seedSet) {
			Randomizer::init(this->seed);
		}
		else {
			Randomizer::init();
		}

		Puzzle* P = new Puzzle();

		/* Generate Nodes and add to puzzle P*/
		if (numberNodes == 0) numberNodes = objects.size();
		Vec<Node*> nodes = generateNodes(objects, numberNodes);
		for (auto& it: nodes) {
			P->addNode(it, true);
		}

		/* Generate Relation and add to Puzzle P */
		Relation R = generateRelationExperimental(P, P->getNodes(), rules);

		P->setRelation(R);

		cleanupNodes(P);

		initializeActivePropertyOnNodes(P);

		return P;
	}

	PPG::Puzzle* Generator::generatePuzzle(Context& context)
	{
		Vec<Ptr<Object>>& objects = context.getObjects();
		Vec<Ptr<Event>>& events = context.getEvents();
		Vec<Rule>& rules = context.getRules();

		return generatePuzzle(objects, events, rules);
	}

	void Generator::removeNodeFromList(Node* N, NodeVec& nodes) {
		NodeVec::iterator found = std::find(nodes.begin(), nodes.end(), N);
		if (found != nodes.end()) {
			nodes.erase(found);
		}
	}


	/**
	*	In order to dont have two meta-equal nodes O1 and O2, where O1 has dependencies and O2 does NOT,
	*	this method will perform a cleanup of the nodes after the relation is generated.
	*
	*/

	void Generator::cleanupNodes(Puzzle* P) {
		NodeVec nodes = P->getNodes();
		NodeVec nodesToDelete;
		Relation R = P->getRelation();

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
		P->setNodes(nodes);
	}


	/*
	*	## MAIN GENERATION METHOD ##
	*	Generating a relation of given nodes;
	*	by keeping invariants and thus solvability within this system
	*/
	Relation Generator::generateRelation(NodeVec nodes, RuleVec rules)
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
			Node* N1 = it;

			/*
			* checks for basic rules (exclusive dependency, equality, circular dependency etc) and for custom rules
			*/
			NodeVec compList = GeneratorHelper::filterCompatibleNodes(N1, rel, nodes, rules);

			// returns nullptr if list is empty
			Node* N2 = Randomizer::getRandomFromList(compList);

			if (N2 != nullptr) {
				rel.addPair(N1, N2);
			}
		}

		return rel;
	}

	PPG::Relation Generator::generateRelationExperimental(Puzzle* P, NodeVec nodes, RuleVec& rules)
	{
		Relation rel;

		NodeVec nodesInGraph;

		Node* NStart = Randomizer::getRandomFromList(nodes);
		nodesInGraph.push_back(NStart);

		for (auto& it: nodes) {
			Node* N1 = it;

			// Copies nodesInGraph
			// Add N1 temporary for filterCompatibleNodes
			NodeVec tmpNodes = nodesInGraph;
			tmpNodes.push_back(N1);

			/*
			* checks for basic rules (exclusive dependency, equality, circular dependency etc) and for custom rules
			*/
			NodeVec compList = GeneratorHelper::filterCompatibleNodes(N1, rel, tmpNodes, rules);

			// returns nullptr if list is empty
			Node* N2 = Randomizer::getRandomFromList(compList);

			if (N2 != nullptr) {
				rel.addPair(N1, N2);
				nodesInGraph.push_back(N1);
			}
		}

		P->setNodes(nodesInGraph);

		return rel;
	}

	NodeVec Generator::generateNodes(ObjVec objects, size_t numNodes)
	{
		NodeVec nodes;

		for (size_t i = 0; i < numNodes; i++) {
			Ptr<Object> obj = Randomizer::getRandomFromList(objects);
			if (obj == nullptr) continue;
			try {
				State state = Randomizer::getRandomFromList(obj->getReachableStates());
				Node* newNode = new Node(obj, state);
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
	void Generator::initializeActivePropertyOnNodes(Puzzle* P)
	{
		NodeVec nodes = P->getNodes();

		for (auto& it: nodes) {
			if (!P->getRelation().hasPrecedingNode(it)) {
				it->setPuzzleNodeState(ENodeState::ACTIVE);
			}
		}
	}

}
