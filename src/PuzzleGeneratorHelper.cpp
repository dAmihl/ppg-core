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

		PuzzleLogger::log("Checking Rule " + r->getTextualRepresentation());
		PuzzleLogger::log("S: " + S->getSimpleTextualRepresentation());
		PuzzleLogger::log("N: " + N->getSimpleTextualRepresentation());

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
			FN = &PuzzleGeneratorHelper::_checkCompatibilityRuleType__BEFORE;
			strict = true;
			break;
		}

		// Call function pointer FN defined in this object
		if (FN == NULL) continue;
		if ((*FN)(nodes, S, N, R, *r, strict) == false) {
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
*
*	This ruletype works a little different.
*	IF node S is LHS of rule, S is only allowed to connect to another node, if S already has RHS as a precing node.
*	If the rule is strict, S needs a RHS as a directly preceding node.
*
*	If S is not allowed to connect with another node N without the RHS of the rule, then S will never be a part of the graph and will be removed as a cleanup.
*	S then only has a chance to be connected to the graph, if a Node N == RHS of the rule is connected to this node.
*
*	IF N is the RHS of the rule, then S has to be the LHS (strict) or have LHS as a preceding node (not strict)
*
*	IF S is not LHS of the rule and N is not the RHS, then the inverse rule must apply, i.e. (N < S) or (N <! S)
*/
bool PuzzleGeneratorHelper::_checkCompatibilityRuleType__AFTER(T_PuzzleNodeList nodes, PuzzleNode* S, PuzzleNode* N, PuzzleRelation* R, PuzzleRule rule, bool isStrict)
{
	/* */
	PuzzleObject* lhsO = rule.getLeftHandSideObject();
	PuzzleState* lhsS = rule.getLeftHandSideState();

	PuzzleObject* rhsO = rule.getRightHandSideObject();
	PuzzleState* rhsS = rule.getRightHandSideState();

	// Find all existing Right-Hand-side nodes of rule
	T_PuzzleNodeList existingRHS = R->findNodesByPattern(nodes, rhsO, rhsS, PuzzleGeneratorHelper::__isRuleObjectEqual, PuzzleGeneratorHelper::__isRuleStateEqual);
	T_PuzzleNodeList existingLHS = R->findNodesByPattern(nodes, lhsO, lhsS, PuzzleGeneratorHelper::__isRuleObjectEqual, PuzzleGeneratorHelper::__isRuleStateEqual);

	if (__isRuleObjectEqual(lhsO, S->getRelatedObject()) && __isRuleStateEqual(lhsS, &(S->getGoalState()))) {
		
		if (isStrict) {
			for (T_PuzzleNodeList::iterator n = existingRHS.begin(); n != existingRHS.end(); ++n) {
				if (!R->findDirectlyPrecedingNode(N, *n)) {
					PuzzleLogger::log("LHS = S and RHS = N; There is no directly preceding RHS found; FALSE");
					return false;
				}
			}
		}
		else {
			for (T_PuzzleNodeList::iterator n = existingRHS.begin(); n != existingRHS.end(); ++n) {
				if (!R->findPrecedingNode(N, *n, false)) {
					PuzzleLogger::log("LHS = S and RHS = N; There is no preceding RHS found; FALSE");
					return false;
				}
			}
		}
	}
	// S is for sure not the LHS
	// if N is the LHS, then S has to either be RHS (strict) or has RHS as a preceding node
	else if (__isRuleObjectEqual(lhsO, N->getRelatedObject()) && __isRuleStateEqual(lhsS, &(N->getGoalState()))) {
		
		// Strict: S has to be RHS
		if (isStrict) {
			return __isRuleObjectEqual(rhsO, S->getRelatedObject()) && __isRuleStateEqual(rhsS, &(S->getGoalState()));
		}
		// Otherwise, S has to atleast have RHS as a preceding node
		else {
			for (T_PuzzleNodeList::iterator n = existingRHS.begin(); n != existingRHS.end(); ++n) {
				if (!R->findPrecedingNode(N, *n, false)) {
					PuzzleLogger::log("RHS = S and LHS = N; There is no preceding RHS found in S; FALSE");
					return false;
				}
			}
			return true;
		}
	}
	else {
		//return true;
		PuzzleRule::E_PuzzleRuleType tmpType = isStrict ? PuzzleRule::STRICT_BEFORE : PuzzleRule::BEFORE;
		PuzzleRule* tmpRule = new PuzzleRule(rule.getRightHandSideObject(), rule.getRightHandSideState(), rule.getLeftHandSideObject(), rule.getLeftHandSideState(), tmpType);
		return _checkCompatibilityRuleType__BEFORE(nodes, N, S, R, *tmpRule, isStrict);
	}
}


/*
*	S = Node to be added from Generator
*	Check if pair (S,N) is compatible by Rule rule
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

	// LHS = N and RHS = S
	// S < N automatically false
	if (PuzzleGeneratorHelper::__isRuleObjectEqual(lhsO, N->getRelatedObject()) && PuzzleGeneratorHelper::__isRuleStateEqual(lhsS, &(N->getGoalState())) &&
		PuzzleGeneratorHelper::__isRuleObjectEqual(rhsO, S->getRelatedObject()) && PuzzleGeneratorHelper::__isRuleStateEqual(rhsS, &(S->getGoalState()))) {
		PuzzleLogger::log("LHS = N and RHS = S; FALSE");
		return false;
	}

	// Find all existing Right-Hand-side nodes of rule
	T_PuzzleNodeList existingRHS = R->findNodesByPattern(nodes, rhsO, rhsS, PuzzleGeneratorHelper::__isRuleObjectEqual, PuzzleGeneratorHelper::__isRuleStateEqual);
	T_PuzzleNodeList existingLHS = R->findNodesByPattern(nodes, lhsO, lhsS, PuzzleGeneratorHelper::__isRuleObjectEqual, PuzzleGeneratorHelper::__isRuleStateEqual);

	// LHS = S and RHS = N
	// S < N for all N
	// In case of a strict rule, ONLY S<N is allowed.
	if (__isRuleObjectEqual(lhsO, S->getRelatedObject()) && __isRuleStateEqual(lhsS, &(S->getGoalState()))){
		
		if (__isRuleObjectEqual(rhsO, N->getRelatedObject()) && __isRuleStateEqual(rhsS, &(N->getGoalState()))) {
			// check if this is the smallest occurance of N? so S is not after any other occurance of N
			for (T_PuzzleNodeList::iterator n = existingRHS.begin(); n != existingRHS.end(); ++n) {
				if (R->findPrecedingNode(N, *n, false)) {
					PuzzleLogger::log("LHS = S and RHS = N; There is a smaller RHS found; FALSE");
					return false;
				}
			}
			PuzzleLogger::log("LHS = S and RHS = N; TRUE");
			return true;
		}
		else if (isStrict) {
			PuzzleLogger::log("LHS = S and RHS != N; STRICT=true; FALSE");
			return false;
		}
			
	}
	
	// Check if S is RHS or has RHS as a preceding node
	for (T_PuzzleNodeList::iterator n = existingRHS.begin(); n != existingRHS.end(); ++n) {
		if (R->findPrecedingNode(S, *n, true)) {
			// Then N is not allowed to be LHS or to have LHS as a following node
			for (T_PuzzleNodeList::iterator k = existingLHS.begin(); k != existingLHS.end(); ++k) {
				if (R->findFollowingNode(N, *k, true)) {
					PuzzleLogger::log("S is RHS or has RHS as a preceding node - N is LHS or has LHS as following node; FALSE");
					return false;
				}
			}
		}
	}

	// Check if S is LHS or has LHS as a preceding node
	for (T_PuzzleNodeList::iterator n = existingLHS.begin(); n != existingLHS.end(); ++n) {
		if (R->findPrecedingNode(S, *n, true)) {
			// Then N is not allowed to have RHS as a preceding node
			for (T_PuzzleNodeList::iterator k = existingRHS.begin(); k != existingRHS.end(); ++k) {
				if (R->findPrecedingNode(N, *k, false)) {
					PuzzleLogger::log("S is LHS or has LHS as a preceding node - N has RHS as preceding node; FALSE");
					return false;
				}
			}
		}
	}

	// Check if N is LHS or has LHS as a following node
	for (T_PuzzleNodeList::iterator n = existingLHS.begin(); n != existingLHS.end(); ++n) {
		if (R->findFollowingNode(N, *n, true)) {
			// Then S is not allowed to be RHS or have RHS as a preceding node
			for (T_PuzzleNodeList::iterator k = existingRHS.begin(); k != existingRHS.end(); ++k) {
				if (R->findPrecedingNode(S, *k, true)) {
					PuzzleLogger::log("N is LHS or has LHS as a preceding node - S is RHS or has RHS as preceding node; FALSE");
					return false;
				}
			}
		}
	}
	
	PuzzleLogger::log("Node is compatible! TRUE");
	return true;

}


bool PuzzleGeneratorHelper::__isRuleObjectEqual(PuzzleObject* o1, PuzzleObject* o2) {
	if (o1 == nullptr || o2 == nullptr) return true;
	return (o1->sameTemplateAs(*o2));
}

bool PuzzleGeneratorHelper::__isRuleStateEqual(PuzzleState* s1, PuzzleState* s2) {
	if (s1 == nullptr || s2 == nullptr) return true;
	return (s1->getStateName() == s2->getStateName());
}
