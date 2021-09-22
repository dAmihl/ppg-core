#include "Generator.h"

namespace PPG {

	Puzzle* Generator::generatePuzzle(ObjVec objects, EventVec events, RuleVec rules)
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

		if (this->NUM_NODES == 0) {
			this->NUM_NODES = (int)objects.size();
		}

		/* Generate Nodes and add to puzzle P*/
		std::vector<Node*> nodes = generateNodes(objects);
		for (std::vector<Node*>::iterator it = nodes.begin(); it != nodes.end(); ++it) {
			P->addNode(*it, true);
		}

		/* Add Events to Puzzle P */
		for (std::vector<Event*>::iterator it = events.begin(); it != events.end(); ++it) {
			P->addEvent((*it));
		}

		/* Generate Relation and add to Puzzle P */
		//Relation *R = __simple_generateRelation(P->getNodes());
		//Relation *R = generateRelation(P->getNodes(), rules);
		Relation* R = generateRelationExperimental(P, P->getNodes(), rules);

		P->setRelation(*R);

		cleanupNodes(P);

		initializeActivePropertyOnNodes(P);

		return P;
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

		for (NodeVec::iterator it = nodes.begin(); it != nodes.end(); ++it) {
			if (!R.hasFollowingNode(*it) && !R.hasPrecedingNode(*it)) {
				for (NodeVec::iterator find = nodes.begin(); find != nodes.end(); ++find) {
					if (*find != *it && GeneratorHelper::checkEquality(*it, *find)) {
						nodesToDelete.push_back(*it);
					}
				}
			}
		}

		for (NodeVec::iterator it = nodesToDelete.begin(); it != nodesToDelete.end(); ++it) {
			removeNodeFromList(*it, nodes);
		}
		P->setNodes(nodes);
	}


	/*
	*
	*	Simple sequential Relation of the given nodes.
	*	Used for testing purposes
	*/
	Relation* Generator::simpleGenerateRelation(NodeVec nodes)
	{
		Relation* rel = new Relation();
		for (std::vector<Node*>::iterator it = nodes.begin(); it != nodes.end() - 1; ++it) {
			Node* N1 = *it;
			Node* N2 = *(it + 1);
			//if (it == nodes.begin()) N1->setPuzzleNodeState(PUZZLENODE_STATE::ACTIVE);
			Pair<Node*, Node*> pair = Relation::makePuzzlePair(N1, N2);
			if (!GeneratorHelper::checkEquality(N1, N2)) {
				rel->addPair(pair);
				if (GeneratorHelper::checkCreatesCircularDependency(pair, rel) || GeneratorHelper::checkCreatesExclusiveDependency(pair, rel) ||
					GeneratorHelper::checkMetaEqualOccurance(pair, rel)) {
					rel->removePair(pair);// TODO
				}
			}// else discard

		}

		return rel;
	}



	/*
	*	## MAIN GENERATION METHOD ##
	*	Generating a relation of given nodes;
	*	by keeping invariants and thus solvability within this system
	*/
	Relation* Generator::generateRelation(NodeVec nodes, RuleVec rules)
	{
		Relation* rel = new Relation();

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

		for (std::vector<Node*>::iterator it = nodes.begin(); it != nodes.end(); ++it) {
			Node* N1 = *it;

			/*
			* checks for basic rules (exclusive dependency, equality, circular dependency etc) and for custom rules
			*/
			NodeVec compList = GeneratorHelper::filterCompatibleNodes(N1, rel, nodes, rules);

			// returns nullptr if list is empty
			Node* N2 = Randomizer::getRandomNodeFromList(compList);

			if (N2 != nullptr) {
				rel->addPair(N1, N2);
			}
		}

		return rel;
	}

	Relation* Generator::generateRelationExperimental(Puzzle* P, NodeVec nodes, RuleVec rules)
	{
		Relation* rel = new Relation();

		NodeVec nodesInGraph;

		Node* NStart = Randomizer::getRandomNodeFromList(nodes);
		nodesInGraph.push_back(NStart);

		for (std::vector<Node*>::iterator it = nodes.begin(); it != nodes.end(); ++it) {
			Node* N1 = *it;

			// Copies nodesInGraph
			// Add N1 temporary for filterCompatibleNodes
			NodeVec tmpNodes = nodesInGraph;
			tmpNodes.push_back(N1);

			/*
			* checks for basic rules (exclusive dependency, equality, circular dependency etc) and for custom rules
			*/
			NodeVec compList = GeneratorHelper::filterCompatibleNodes(N1, rel, tmpNodes, rules);

			// returns nullptr if list is empty
			Node* N2 = Randomizer::getRandomNodeFromList(compList);

			if (N2 != nullptr) {
				rel->addPair(N1, N2);
				nodesInGraph.push_back(N1);
			}
		}

		P->setNodes(nodesInGraph);

		return rel;
	}

	NodeVec Generator::generateNodes(ObjVec objects)
	{

		NodeVec nodes;

		for (int i = 0; i < this->NUM_NODES; i++) {
			Object* obj = Randomizer::getRandomFromList(objects);
			if (obj == nullptr) continue;
			try {
				State state = Randomizer::getRandomStateFromList(obj->getReachableStates());
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

	void Generator::setNumberNodes(int n)
	{
		this->NUM_NODES = n;
	}

	int Generator::getNumberNodes()
	{
		return this->NUM_NODES;
	}

	void Generator::setSeed(unsigned int seed)
	{
		this->seed = seed;
		this->seedSet = true;
	}

	unsigned int Generator::getSeed()
	{
		return this->seed;
	}




	/*
	*	Initialize all ACTIVE nodes with state ACTIVE
	*	Remember: Only active nodes are initially able to handle events
	*/
	void Generator::initializeActivePropertyOnNodes(Puzzle* P)
	{

		NodeVec nodes = P->getNodes();

		for (NodeVec::iterator it = nodes.begin(); it != nodes.end(); ++it) {
			if (!P->getRelation().hasPrecedingNode(*it)) {
				(*it)->setPuzzleNodeState(PUZZLENODE_STATE::ACTIVE);
			}
		}


	}

}
