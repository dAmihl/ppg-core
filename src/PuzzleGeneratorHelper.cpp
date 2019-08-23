#include "PuzzleGeneratorHelper.h"


/*
*	True: Circular Dependency detected
*	False: No Circular Dependency with new pair P in Relation R detected
*/
bool PuzzleGeneratorHelper::_checkCreatesCircularDependency(T_PuzzleNodePair P, PuzzleRelation* R)
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
bool PuzzleGeneratorHelper::_checkCreatesExclusiveDependency(T_PuzzleNodePair P, PuzzleRelation* R)
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
bool PuzzleGeneratorHelper::_findNodeSequential(PuzzleNode* N, T_PuzzleNodePair start, PuzzleRelation* R)
{
	// Check starting node if direct successor
	if (start.second == N) return true;

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

bool PuzzleGeneratorHelper::_checkEquality(PuzzleNode* N1, PuzzleNode* N2)
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

bool PuzzleGeneratorHelper::checkMetaEqualOccuranceByNode(PuzzleNode* N, PuzzleRelation* R) {

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

bool PuzzleGeneratorHelper::_checkMetaEqualOccurance(T_PuzzleNodePair P, PuzzleRelation* R)
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
T_PuzzleNodeList PuzzleGeneratorHelper::_filterCompatibleNodes(PuzzleNode* N, PuzzleRelation* R, T_PuzzleNodeList nodes, T_PuzzleRuleList rules)
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

bool PuzzleGeneratorHelper::_checkCompatibilityBasicRules(PuzzleNode* S, PuzzleNode* N, PuzzleRelation* R)
{
	T_PuzzleNodePair pair = PuzzleRelation::makePuzzlePair(S, N);
	if (!_checkEquality(S, N)) { // check node equality
		R->addPair(pair);
		if (PuzzleGeneratorHelper::_checkCreatesCircularDependency(pair, R) || PuzzleGeneratorHelper::_checkCreatesExclusiveDependency(pair, R) ||
			PuzzleGeneratorHelper::_checkMetaEqualOccurance(pair, R)) {
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
bool PuzzleGeneratorHelper::_checkCompatibilityCustomRules(T_PuzzleNodeList nodes, PuzzleNode* S, PuzzleNode* N, PuzzleRelation* R, T_PuzzleRuleList rules)
{
	for (T_PuzzleRuleList::iterator r = rules.begin(); r != rules.end(); ++r) {

		bool (*FN) (T_PuzzleNodeList, PuzzleNode*, PuzzleNode*, PuzzleRelation*, PuzzleRule, bool) = NULL;
		bool strict = false;

		switch (r->getRuleType()) {
		case PuzzleRule::E_PuzzleRuleType::AFTER:
			FN = &PuzzleGeneratorHelper::_checkCompatibilityRuleType__AFTER;
			strict = false;
			break;
		case PuzzleRule::E_PuzzleRuleType::STRICT_AFTER:
			FN = &PuzzleGeneratorHelper::_checkCompatibilityRuleType__AFTER;
			strict = true;
			break;
		case PuzzleRule::E_PuzzleRuleType::BEFORE:
			FN = &PuzzleGeneratorHelper::_checkCompatibilityRuleType__BEFORE;
			strict = false;
			break;
		case PuzzleRule::E_PuzzleRuleType::STRICT_BEFORE:
			FN = &PuzzleGeneratorHelper::_checkCompatibilityRuleType__AFTER;
			strict = true;
			break;
		}

		// Call function pointer FN defined in this object
		if (FN == NULL) continue;
		if (!(*FN)(nodes, S, N, R, *r, strict)) {
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
bool PuzzleGeneratorHelper::_checkCompatibilityRuleType__AFTER(T_PuzzleNodeList nodes, PuzzleNode* S, PuzzleNode* N, PuzzleRelation* R, PuzzleRule rule, bool isStrict)
{
	return true;
}


/*
*	S = Node to be added from Generator
*	PuzzleRule::Type BEFORE
*	Check "STRICT_BEFORE" by setting isStrict to true!
*	True = Compatible!
*	False = INCOMPATIBLE!
*/
bool PuzzleGeneratorHelper::_checkCompatibilityRuleType__BEFORE(T_PuzzleNodeList nodes, PuzzleNode* S, PuzzleNode* N, PuzzleRelation* R, PuzzleRule rule, bool isStrict)
{
	/* */
	PuzzleObject* lhsO = rule.getLeftHandSideObject();
	PuzzleState* lhsS = rule.getLeftHandSideState();

	PuzzleObject* rhsO = rule.getRightHandSideObject();
	PuzzleState* rhsS = rule.getRightHandSideState();

	// LHS = S and RHS = N
	if (__isRuleObjectEqual(lhsO, S->getRelatedObject()) && __isRuleStateEqual(lhsS, &(S->getGoalState())) &&
		__isRuleObjectEqual(rhsO, N->getRelatedObject()) && __isRuleStateEqual(rhsS, &(N->getGoalState()))) {

		T_PuzzleNodeList existingRHS = R->findNodesByPattern(nodes, rhsO, rhsS, PuzzleGeneratorHelper::__isRuleObjectEqual, PuzzleGeneratorHelper::__isRuleStateEqual);

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

	// LHS = S
	if (PuzzleGeneratorHelper::__isRuleObjectEqual(lhsO, S->getRelatedObject()) && PuzzleGeneratorHelper::__isRuleStateEqual(lhsS, &(S->getGoalState()))) {
		/*
			Get every existing nodes which fullfill Right Hand Side of Rule
		*/
		T_PuzzleNodeList existingRHS = R->findNodesByPattern(nodes, rhsO, rhsS, __isRuleObjectEqual, PuzzleGeneratorHelper::__isRuleStateEqual);
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
	* RHS = S
	* If the node S is going to get attached, and S is Right hand side in Rule
	*	Then check if there is no LHS following
	*/
	if (PuzzleGeneratorHelper::__isRuleObjectEqual(rhsO, S->getRelatedObject()) && PuzzleGeneratorHelper::__isRuleStateEqual(rhsS, &(S->getGoalState()))) {
		/*
		Get every existing nodes which fullfill Left Hand Side of Rule
		*/
		T_PuzzleNodeList existingLHS = R->findNodesByPattern(nodes, lhsO, lhsS, PuzzleGeneratorHelper::__isRuleObjectEqual, PuzzleGeneratorHelper::__isRuleStateEqual);
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
	if (PuzzleGeneratorHelper::__isRuleObjectEqual(lhsO, N->getRelatedObject()) && PuzzleGeneratorHelper::__isRuleStateEqual(lhsS, &(N->getGoalState()))) {
		/*
		Get every existing nodes which fullfill Right Hand Side of Rule
		*/
		T_PuzzleNodeList existingRHS = R->findNodesByPattern(nodes, rhsO, rhsS, PuzzleGeneratorHelper::__isRuleObjectEqual, PuzzleGeneratorHelper::__isRuleStateEqual);
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
	if (PuzzleGeneratorHelper::__isRuleObjectEqual(rhsO, N->getRelatedObject()) && PuzzleGeneratorHelper::__isRuleStateEqual(rhsS, &(N->getGoalState()))) {
		/*
		Get every existing nodes which fullfill Right Hand Side of Rule
		*/
		T_PuzzleNodeList existingRHS = R->findNodesByPattern(nodes, lhsO, lhsS, PuzzleGeneratorHelper::__isRuleObjectEqual, PuzzleGeneratorHelper::__isRuleStateEqual);
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


bool PuzzleGeneratorHelper::__isRuleObjectEqual(PuzzleObject* o1, PuzzleObject* o2) {
	if (o1 == nullptr || o2 == nullptr) return false;
	return (o1->sameTemplateAs(*o2));
}

bool PuzzleGeneratorHelper::__isRuleStateEqual(PuzzleState* s1, PuzzleState* s2) {
	if (s1 == nullptr || s2 == nullptr) return true;
	return (s1->getStateName() == s2->getStateName());
}
