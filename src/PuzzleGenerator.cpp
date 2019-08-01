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
				if (*find != *it && _checkEquality(*it, *find)) {
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
		if (!_checkEquality(N1, N2)) {
			rel->addPair(pair);
			if (_checkCreatesCircularDependency(pair, rel) || _checkCreatesExclusiveDependency(pair, rel)||
				_checkMetaEqualOccurance(pair, rel)) {
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
		T_PuzzleNodeList compList = _filterCompatibleNodes(N1, rel, nodes, rules);

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
*	True: Circular Dependency detected
*	False: No Circular Dependency with new pair P in Relation R detected
*/
bool PuzzleGenerator::_checkCreatesCircularDependency(T_PuzzleNodePair P, PuzzleRelation * R)
{

	T_PuzzlePairList pairs = R->getPairs();

	bool result = false;

	/*
	* Get first successor of pair P.
	* Then search sequentially for a reoccurance of P->First, which causes a circular dependency
	*/
	for (T_PuzzlePairList::iterator it = pairs.begin(); it != pairs.end(); ++it) {
		if (it->first == P.second) {
			if (_findNodeSequential(P.first, *it, R)) {
				return true;
			}
		}
	}

	return false;
}

/*
*	True: Exclusive Dependency detected
*	False: No Exclusive Dependency with new pair P in Relation R detected
*	Extension: Parallel nodes must have different related game-objects
*/
bool PuzzleGenerator::_checkCreatesExclusiveDependency(T_PuzzleNodePair P, PuzzleRelation * R)
{
	T_PuzzlePairList parallelPairs = R->getParallelPairs(P);

	for (T_PuzzlePairList::iterator it = parallelPairs.begin(); it != parallelPairs.end(); ++it) {
		
		if (it->first->getRelatedObject() == P.first->getRelatedObject()) {
			return true;
		}
	}

	return false;
}




/*
*	Trys to find occurence of N going from /start/ sequential till the end
*/
bool PuzzleGenerator::_findNodeSequential(PuzzleNode * N, T_PuzzleNodePair start, PuzzleRelation * R)
{
	T_PuzzlePairList nextPairs = R->getNextPairs(start);

	if (nextPairs.size() < 1) return false;

	bool result = false;

	for (T_PuzzlePairList::iterator it = nextPairs.begin(); it != nextPairs.end(); ++it) {
		if (/*it->first == N || */it->second == N) {
			return true;
		}
		else {
			if (_findNodeSequential(N, *it, R)) {
				return true;
			}
		}
	}

	return false;
}

bool PuzzleGenerator::_checkEquality(PuzzleNode * N1, PuzzleNode * N2)
{
	return N1->getRelatedObject() == N2->getRelatedObject() &&
		N1->getGoalState().getStateName() == N2->getGoalState().getStateName();
}


/*
*	Checks occurance of the next "meta-equal" node within the relation
*	Meta-equal means: Same object, same State, but different Memoryadress (different Node)
*	It is not allowed for two "meta-equal" nodes to be "directly" connected
*	e.g. N1(Obj1, S1) -> .. -> N2(Obj1, S1) :: Is not allowed!
*	but N1(Obj1, S1) -> ... -> N3(Obj1, S2) --> ... --> N2(Obj1, S1) :: IS ALLOWED!
*/

bool checkMetaEqualOccuranceByNode(PuzzleNode* N, PuzzleRelation *R) {
	
	bool result = false;
	T_PuzzleNodeList nextMetas = R->findNearestFollowingEqualNodesByObject(N);

	for (T_PuzzleNodeList::iterator it = nextMetas.begin(); it != nextMetas.end(); ++it) {
		if ((*it)->getGoalState().getStateName() == N->getGoalState().getStateName()) {
			return true;
		}
	}

	T_PuzzleNodeList prevMetas = R->findNearestPrecedingEqualNodesByObject(N);

	for (T_PuzzleNodeList::iterator it = prevMetas.begin(); it != prevMetas.end(); ++it) {
		if ((*it)->getGoalState().getStateName() == N->getGoalState().getStateName()) {
			return true;
		}
	}


	return result;
}

bool PuzzleGenerator::_checkMetaEqualOccurance(T_PuzzleNodePair P, PuzzleRelation * R)
{
	bool result = false;

	result = checkMetaEqualOccuranceByNode(P.first, R) || checkMetaEqualOccuranceByNode(P.second, R);

	return result;
}




/*
*	Filters node-list nodes by compatibility
*		- by basic rules (Exclusive / Circular dependency, direct equality, etc)
*		- and by custom rules
*
*
*/
T_PuzzleNodeList PuzzleGenerator::_filterCompatibleNodes(PuzzleNode * N, PuzzleRelation * R, T_PuzzleNodeList nodes, T_PuzzleRuleList rules)
{
	T_PuzzleNodeList compatibles;

	for (T_PuzzleNodeList::iterator it = nodes.begin(); it != nodes.end(); ++it) {
		if (_checkCompatibilityBasicRules(N, *it, R)) {
			if (_checkCompatibilityCustomRules(nodes, N, *it, R, rules)) {
				compatibles.push_back(*it);
			}
		}
	}
	return compatibles;
}



/*
*	Checks compatibility of two nodes S and N by Basic Rules (PuzzleRule)s
*/

bool PuzzleGenerator::_checkCompatibilityBasicRules(PuzzleNode* S, PuzzleNode *N, PuzzleRelation * R)
{
	T_PuzzleNodePair pair = PuzzleRelation::makePuzzlePair(S, N);
	if (!_checkEquality(S, N)) { // check node equality
		R->addPair(pair);
		if (_checkCreatesCircularDependency(pair, R) || _checkCreatesExclusiveDependency(pair, R) ||
			_checkMetaEqualOccurance(pair, R)) {
			R->removePair(pair);// TODO
			return false;
		}
		else {
			R->removePair(pair); // TODO ? 
			return true;
		}
	}// else discard

	return false;
}


/*
*	Checks compatibility of two nodes S and N by User-made Custom Rules (PuzzleRule)s
*/
bool PuzzleGenerator::_checkCompatibilityCustomRules(T_PuzzleNodeList nodes, PuzzleNode * S, PuzzleNode * N, PuzzleRelation * R, T_PuzzleRuleList rules)
{
	for (T_PuzzleRuleList::iterator r = rules.begin(); r != rules.end(); ++r) {

		bool (PuzzleGenerator::*FN) (T_PuzzleNodeList, PuzzleNode*, PuzzleNode*, PuzzleRelation*, PuzzleRule, bool) = NULL;
		bool strict = false;

		switch (r->getRuleType()) {
			case PuzzleRule::E_PuzzleRuleType::AFTER: 
				FN = &PuzzleGenerator::_checkCompatibilityRuleType__AFTER;
				strict = false;
				break;
			case PuzzleRule::E_PuzzleRuleType::STRICT_AFTER:
				FN = &PuzzleGenerator::_checkCompatibilityRuleType__AFTER;
				strict = true;
				break;
			case PuzzleRule::E_PuzzleRuleType::BEFORE:
				FN = &PuzzleGenerator::_checkCompatibilityRuleType__BEFORE;
				strict = false;
				break;
			case PuzzleRule::E_PuzzleRuleType::STRICT_BEFORE: 
				FN = &PuzzleGenerator::_checkCompatibilityRuleType__AFTER;
				strict = true;
				break;
		}

		// Call function pointer FN defined in this object
		if (FN == NULL) continue;
		if (!(*this.*FN)(nodes, S, N, R, *r, strict)) {
			return false;
		}

	}
	return true;
}


/*
*	PuzzleRule::Type AFTER
*	Check "STRICT_AFTER" by setting isStrict to true!
*	True = Compatible!
*	False = INCOMPATIBLE!
*/
bool PuzzleGenerator::_checkCompatibilityRuleType__AFTER(T_PuzzleNodeList nodes, PuzzleNode * S, PuzzleNode * N, PuzzleRelation * R, PuzzleRule rule, bool isStrict)
{
	return true;
}





bool __isRuleObjectEqual(PuzzleObject* o1, PuzzleObject* o2) {
	if (o1 == nullptr || o2 == nullptr) return false;
	return (o1->sameTemplateAs(*o2));
}

bool __isRuleStateEqual(PuzzleState* s1, PuzzleState* s2) {
	if (s1 == nullptr || s2 == nullptr) return true;
	return (s1->getStateName() == s2->getStateName());
}

/*
*	PuzzleRule::Type BEFORE
*	Check "STRICT_BEFORE" by setting isStrict to true!
*	True = Compatible!
*	False = INCOMPATIBLE!
*/
bool PuzzleGenerator::_checkCompatibilityRuleType__BEFORE(T_PuzzleNodeList nodes, PuzzleNode * S, PuzzleNode * N, PuzzleRelation * R, PuzzleRule rule, bool isStrict)
{
	/* */
	PuzzleObject* lhsO = rule.getLeftHandSideObject();
	PuzzleState* lhsS = rule.getLeftHandSideState();

	PuzzleObject* rhsO = rule.getRightHandSideObject();
	PuzzleState* rhsS = rule.getRightHandSideState();

	// LHS = S und RHS = N
	if (__isRuleObjectEqual(lhsO, S->getRelatedObject()) && __isRuleStateEqual(lhsS, &(S->getGoalState())) &&
		__isRuleObjectEqual(rhsO, N->getRelatedObject()) && __isRuleStateEqual(rhsS, &(N->getGoalState()))) {
		
		T_PuzzleNodeList existingRHS = R->findNodesByPattern(nodes, rhsO, rhsS, __isRuleObjectEqual, __isRuleStateEqual);
		
		// check if this is the smallest occurance of N? so S is not after any other occurance of N
		for (T_PuzzleNodeList::iterator n = existingRHS.begin(); n != existingRHS.end(); ++n) {
			if (R->findPrecedingNode(N, *n)) {
				return false;
			}
		}
		return true; 
	}
	else {
		if (isStrict) {
			return false;
		}
	}

	if (__isRuleObjectEqual(lhsO, S->getRelatedObject()) && __isRuleStateEqual(lhsS, &(S->getGoalState()))) {
		/*
			Get every existing nodes which fullfill Right Hand Side of Rule
		*/
		T_PuzzleNodeList existingRHS = R->findNodesByPattern(nodes, rhsO, rhsS, __isRuleObjectEqual, __isRuleStateEqual);
		bool result = true;
		/*
			For each existing node, check if S will be BEFORE those nodes by trying to find the node in the sequential order
		*/
		for (T_PuzzleNodeList::iterator n = existingRHS.begin(); n != existingRHS.end(); ++n) {
			if (!R->findFollowingNode(N, *n)) {
				return false;
			}
		}
	}
	
	/* If the node S is going to get attached, and S is Right hand side in Rule
	*	Then check if there is no LHS following
	*/
	if (__isRuleObjectEqual(rhsO, S->getRelatedObject()) && __isRuleStateEqual(rhsS, &(S->getGoalState()))) {
		/*
		Get every existing nodes which fullfill Left Hand Side of Rule
		*/
		T_PuzzleNodeList existingLHS = R->findNodesByPattern(nodes, lhsO, lhsS, __isRuleObjectEqual, __isRuleStateEqual);
		bool result = true;
		/*
		For each existing node, check if N will be AFTER those nodes by trying to find the node in the sequential order
		*/
		for (T_PuzzleNodeList::iterator n = existingLHS.begin(); n != existingLHS.end(); ++n) {
			if (!R->findPrecedingNode(N, *n)) {
				return false;
			}
		}
	}

	/*
	*	If N is LHS of Rule, check that there is no preceding RHS
	*/
	if (__isRuleObjectEqual(lhsO, N->getRelatedObject()) && __isRuleStateEqual(lhsS, &(N->getGoalState()))) {
		/*
		Get every existing nodes which fullfill Right Hand Side of Rule
		*/
		T_PuzzleNodeList existingRHS = R->findNodesByPattern(nodes, rhsO, rhsS, __isRuleObjectEqual, __isRuleStateEqual);
		bool result = true;
		/*
		For each existing node, check if S will be BEFORE those nodes by trying to find the node in the sequential order
		*/
		for (T_PuzzleNodeList::iterator n = existingRHS.begin(); n != existingRHS.end(); ++n) {
			if (!R->findFollowingNode(N, *n)) {
				return false;
			}
		}
	}

	/*
	*	If N is RHS of Rule, check that there is no preceding RHS
	*/
	if (__isRuleObjectEqual(rhsO, N->getRelatedObject()) && __isRuleStateEqual(rhsS, &(N->getGoalState()))) {
		/*
		Get every existing nodes which fullfill Right Hand Side of Rule
		*/
		T_PuzzleNodeList existingRHS = R->findNodesByPattern(nodes, lhsO, lhsS, __isRuleObjectEqual, __isRuleStateEqual);
		bool result = true;
		/*
		For each existing node, check if S will be BEFORE those nodes by trying to find the node in the sequential order
		*/
		for (T_PuzzleNodeList::iterator n = existingRHS.begin(); n != existingRHS.end(); ++n) {
			if (!R->findPrecedingNode(S, *n)) {
				return false;
			}
		}
	}
	


	return true;
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




