#include "PuzzleGenerator.h"




PuzzleGenerator::PuzzleGenerator()
{
}


PuzzleGenerator::~PuzzleGenerator()
{
}

Puzzle * PuzzleGenerator::generatePuzzle(T_PuzzleObjectList objects, T_PuzzleEventList events, T_PuzzleRuleList rules)
{

	// initialize randomizer
	PuzzleRandomizer::init();
	Puzzle* P = new Puzzle();

	if (this->NUM_NODES == 0) {
		this->NUM_NODES = (int)objects.size();
	}

	/* Generate Nodes and add to puzzle P*/
	std::vector<PuzzleNode*> nodes = generateNodes(objects);
	for (std::vector<PuzzleNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it) {
		P->addNode(*it, true);
	}

	/* Add Events to Puzzle P */
	for (std::vector<PuzzleEvent*>::iterator it = events.begin(); it != events.end(); ++it) {
		P->addEvent((*it));
	}

	/* Generate Relation and add to Puzzle P */
	//PuzzleRelation *R = __simple_generateRelation(P->getNodes());
	PuzzleRelation *R = generateRelation(P->getNodes(), rules);
	P->setRelation(*R);

	_cleanupNodes(P);

	_initializeActivePropertyOnNodes(P);

	return P;
}


void PuzzleGenerator::_removeNodeFromList(PuzzleNode*N, T_PuzzleNodeList &nodes) {
	T_PuzzleNodeList::iterator found = std::find(nodes.begin(), nodes.end(), N);
	if (found != nodes.end()) {
		nodes.erase(found);
	}
}


/**
*	In order to dont have two meta-equal nodes O1 and O2, where O1 has dependencies and O2 does NOT,
*	this method will perform a cleanup of the nodes after the relation is generated.
*
*/

void PuzzleGenerator::_cleanupNodes(Puzzle* P) {
	T_PuzzleNodeList nodes = P->getNodes();
	T_PuzzleNodeList nodesToDelete;
	PuzzleRelation R = P->getRelation();

	for (T_PuzzleNodeList::iterator it = nodes.begin(); it != nodes.end(); ++it) {
		if (!R.hasFollowingNode(*it) && !R.hasPrecedingNode(*it)) {
			for (T_PuzzleNodeList::iterator find = nodes.begin(); find != nodes.end(); ++find) {
				if (*find != *it && PuzzleGeneratorHelper::_checkEquality(*it, *find)) {
					nodesToDelete.push_back(*it);
				}
			}
		}
	}

	for (T_PuzzleNodeList::iterator it = nodesToDelete.begin(); it != nodesToDelete.end(); ++it) {
		_removeNodeFromList(*it, nodes);
	}
	P->setNodes(nodes);
}


/*
*
*	Simple sequential Relation of the given nodes.
*	Used for testing purposes
*/
PuzzleRelation* PuzzleGenerator::__simple_generateRelation(T_PuzzleNodeList nodes)
{
	PuzzleRelation *rel = new PuzzleRelation();
	for (std::vector<PuzzleNode*>::iterator it = nodes.begin(); it != nodes.end()-1; ++it) {
		PuzzleNode* N1 = *it;
		PuzzleNode* N2 = *(it + 1);
		//if (it == nodes.begin()) N1->setPuzzleNodeState(PUZZLENODE_STATE::ACTIVE);
		T_PuzzleNodePair pair = PuzzleRelation::makePuzzlePair(N1, N2);
		if (!PuzzleGeneratorHelper::_checkEquality(N1, N2)) {
			rel->addPair(pair);
			if (PuzzleGeneratorHelper::_checkCreatesCircularDependency(pair, rel) || PuzzleGeneratorHelper::_checkCreatesExclusiveDependency(pair, rel)||
				PuzzleGeneratorHelper::_checkMetaEqualOccurance(pair, rel)) {
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
PuzzleRelation* PuzzleGenerator::generateRelation(T_PuzzleNodeList nodes,  T_PuzzleRuleList rules)
{
	PuzzleRelation *rel = new PuzzleRelation();
	
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

	for (std::vector<PuzzleNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it) {
		PuzzleNode* N1 = *it;

		/* 
		* checks for basic rules (exclusive dependency, equality, circular dependency etc) and for custom rules
		*/
		T_PuzzleNodeList compList = PuzzleGeneratorHelper::_filterCompatibleNodes(N1, rel, nodes, rules);

		// returns nullptr if list is empty
		PuzzleNode* N2 = PuzzleRandomizer::getRandomNodeFromList(compList);
		//if (it == nodes.begin()) N1->setPuzzleNodeState(PUZZLENODE_STATE::ACTIVE);

		if (N2 != nullptr) {
			rel->addPair(N1, N2);
		}
	}

	return rel;
}

T_PuzzleNodeList PuzzleGenerator::generateNodes(T_PuzzleObjectList objects)
{

	T_PuzzleNodeList nodes;

	for (int i = 0; i < this->NUM_NODES; i++) {
		PuzzleObject* obj = PuzzleRandomizer::getRandomObjectFromList(objects);
		if (obj == nullptr) continue;
		try {
			PuzzleState state = PuzzleRandomizer::getRandomStateFromList(obj->getReachableStates());
			PuzzleNode * newNode = new PuzzleNode(obj, state);
			nodes.push_back(newNode);
		}
		catch (int error) {
			// TODO error handling
			(void)error;
		}
	}
	return nodes;
}

void PuzzleGenerator::setNumberNodes(int n)
{
	this->NUM_NODES = n;
}

int PuzzleGenerator::getNumberNodes()
{
	return this->NUM_NODES;
}




/*
*	Initialize all ACTIVE nodes with state ACTIVE
*	Remember: Only active nodes are initially able to handle events
*/
void PuzzleGenerator::_initializeActivePropertyOnNodes(Puzzle * P)
{

	T_PuzzleNodeList nodes = P->getNodes();

	for (T_PuzzleNodeList::iterator it = nodes.begin(); it != nodes.end(); ++it) {
		if (!P->getRelation().hasPrecedingNode(*it)) {
			(*it)->setPuzzleNodeState(PUZZLENODE_STATE::ACTIVE);
		}
	}


}

