#include "GeneratorHelper.h"


namespace PPG {

	/*
	*	True: Circular Dependency detected
	*	False: No Circular Dependency with new pair P in Relation R detected
	*/
	bool GeneratorHelper::checkCreatesCircularDependency(NodePair P, Relation* R)
	{

		NodePairVec pairs = R->getPairs();

		bool result = false;

		/*
		* Get first successor of pair P.
		* Then search sequentially for a reoccurance of P->First, which causes a circular dependency
		*/
		for (auto& it: pairs) {
			if (it.first == P.second) {
				if (findNodeSequential(P.first, it, R)) {
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
	bool GeneratorHelper::checkCreatesExclusiveDependency(NodePair P, Relation* R)
	{
		NodePairVec parallelPairs = R->getParallelPairs(P);

		for (auto& it: parallelPairs) {

			if (it.first->getRelatedObject() == P.first->getRelatedObject()) {
				return true;
			}
		}

		return false;
	}




	/*
	*	Trys to find occurence of N going from /start/ sequential till the end
	*/
	bool GeneratorHelper::findNodeSequential(Node* N, NodePair start, Relation* R)
	{
		// Check starting node if direct successor
		if (start.second == N) return true;

		NodePairVec nextPairs = R->getNextPairs(start);

		if (nextPairs.size() < 1) return false;

		bool result = false;

		for (auto& it: nextPairs) {
			if (it.second == N) {
				return true;
			}
			else {
				if (findNodeSequential(N, it, R)) {
					return true;
				}
			}
		}

		return false;
	}

	bool GeneratorHelper::checkEquality(Node* N1, Node* N2)
	{
		return N1->getRelatedObject() == N2->getRelatedObject() &&
			*N1->getGoalState() == *N2->getGoalState();
	}


	/*
	*	Checks occurance of the next "meta-equal" node within the relation
	*	Meta-equal means: Same object, same State, but different Memoryadress (different Node)
	*	It is not allowed for two "meta-equal" nodes to be "directly" connected
	*	e.g. N1(Obj1, S1) -> .. -> N2(Obj1, S1) :: Is not allowed!
	*	but N1(Obj1, S1) -> ... -> N3(Obj1, S2) --> ... --> N2(Obj1, S1) :: IS ALLOWED!
	*/

	bool GeneratorHelper::checkMetaEqualOccuranceByNode(Node* N, Relation* R) {

		bool result = false;
		NodeVec nextMetas = R->findNearestFollowingEqualNodesByObject(N);

		for (auto& it: nextMetas){
			if (it->getGoalState() == N->getGoalState()) {
				return true;
			}
		}

		NodeVec prevMetas = R->findNearestPrecedingEqualNodesByObject(N);

		for (auto& it: prevMetas){
			if (it->getGoalState()->getStateName() == N->getGoalState()->getStateName()) {
				return true;
			}
		}


		return result;
	}

	bool GeneratorHelper::checkMetaEqualOccurance(NodePair P, Relation* R)
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
	NodeVec GeneratorHelper::filterCompatibleNodes(Node* N, Relation* R, NodeVec nodes, RuleVec rules)
	{
		NodeVec compatibles;

		for (auto& it: nodes) {
			if (checkCompatibilityBasicRules(N, it, R)) {
				if (checkCompatibilityCustomRules(nodes, N, it, R, rules)) {
					compatibles.push_back(it);
				}
			}
		}
		return compatibles;
	}



	/*
	*	Checks compatibility of two nodes S and N by Basic Rules (Rule)s
	*/

	bool GeneratorHelper::checkCompatibilityBasicRules(Node* S, Node* N, Relation* R)
	{
		Pair<Node*, Node*> pair = Relation::makePuzzlePair(S, N);
		if (!checkEquality(S, N)) { // check node equality
			R->addPair(pair);
			if (GeneratorHelper::checkCreatesCircularDependency(pair, R) || GeneratorHelper::checkCreatesExclusiveDependency(pair, R) ||
				GeneratorHelper::checkMetaEqualOccurance(pair, R)) {
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
	*	Checks compatibility of two nodes S and N by User-made Custom Rules (Rule)s
	*/
	bool GeneratorHelper::checkCompatibilityCustomRules(NodeVec nodes, Node* S, Node* N, Relation* R, RuleVec rules)
	{
		for (auto& r: rules) {

			Logger::log("Checking Rule " + r.getTextualRepresentation());
			Logger::log("S: " + S->getSimpleTextualRepresentation());
			Logger::log("N: " + N->getSimpleTextualRepresentation());

			bool (*FN) (NodeVec, Node*, Node*, Relation*, Rule, bool) = NULL;
			bool strict = false;

			switch (r.getRuleType()) {
			case Rule::EPuzzleRuleType::AFTER:
				FN = &GeneratorHelper::checkCompatibilityRuleTypeAfter;
				strict = false;
				break;
			case Rule::EPuzzleRuleType::STRICT_AFTER:
				FN = &GeneratorHelper::checkCompatibilityRuleTypeAfter;
				strict = true;
				break;
			case Rule::EPuzzleRuleType::BEFORE:
				FN = &GeneratorHelper::checkCompatibilityRuleTypeBefore;
				strict = false;
				break;
			case Rule::EPuzzleRuleType::STRICT_BEFORE:
				FN = &GeneratorHelper::checkCompatibilityRuleTypeBefore;
				strict = true;
				break;
			}

			// Call function pointer FN defined in this object
			if (FN == NULL) continue;
			if ((*FN)(nodes, S, N, R, r, strict) == false) {
				return false;
			}

		}
		return true;
	}


	/*
	*	Rule::Type AFTER
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
	bool GeneratorHelper::checkCompatibilityRuleTypeAfter(NodeVec nodes, Node* S, Node* N, Relation* R, Rule rule, bool isStrict)
	{
		/* */
		const Object* lhsO = rule.getLeftHandSideObject();
		const State* lhsS = rule.getLeftHandSideState();

		const Object* rhsO = rule.getRightHandSideObject();
		const State* rhsS = rule.getRightHandSideState();

		// Find all existing Right-Hand-side nodes of rule
		NodeVec existingRHS = R->findNodesByPattern(nodes, rhsO, rhsS, GeneratorHelper::isRuleObjectEqual, GeneratorHelper::isRuleStateEqual);
		NodeVec existingLHS = R->findNodesByPattern(nodes, lhsO, lhsS, GeneratorHelper::isRuleObjectEqual, GeneratorHelper::isRuleStateEqual);

		if (isRuleObjectEqual(lhsO, S->getRelatedObject()) && isRuleStateEqual(lhsS, S->getGoalState())) {

			if (isStrict) {
				for (auto& n: existingRHS) {
					if (!R->findDirectlyPrecedingNode(N, n)) {
						Logger::log("LHS = S and RHS = N; There is no directly preceding RHS found; FALSE");
						return false;
					}
				}
			}
			else {
				for (auto& n: existingRHS) {
					if (!R->findPrecedingNode(N, n, false)) {
						Logger::log("LHS = S and RHS = N; There is no preceding RHS found; FALSE");
						return false;
					}
				}
			}
		}
		// S is for sure not the LHS
		// if N is the LHS, then S has to either be RHS (strict) or has RHS as a preceding node
		else if (isRuleObjectEqual(lhsO, N->getRelatedObject()) && isRuleStateEqual(lhsS, N->getGoalState())) {

			// Strict: S has to be RHS
			if (isStrict) {
				return isRuleObjectEqual(rhsO, S->getRelatedObject()) && isRuleStateEqual(rhsS, S->getGoalState());
			}
			// Otherwise, S has to atleast have RHS as a preceding node
			else {
				for (auto& n: existingRHS) {
					if (!R->findPrecedingNode(N, n, false)) {
						Logger::log("RHS = S and LHS = N; There is no preceding RHS found in S; FALSE");
						return false;
					}
				}
				return true;
			}
		}
		else {
			//return true;
			Rule::EPuzzleRuleType tmpType = isStrict ? Rule::EPuzzleRuleType::STRICT_BEFORE : Rule::EPuzzleRuleType::BEFORE;
			Rule* tmpRule = new Rule(rule.getRightHandSideObject(), rule.getRightHandSideState(), rule.getLeftHandSideObject(), rule.getLeftHandSideState(), tmpType);
			return checkCompatibilityRuleTypeBefore(nodes, N, S, R, *tmpRule, isStrict);
		}

		return false;
	}


	/*
	*	S = Node to be added from Generator
	*	Check if pair (S,N) is compatible by Rule rule
	*	Rule::Type BEFORE
	*	Check "STRICT_BEFORE" by setting isStrict to true!
	*	True = Compatible!
	*	False = INCOMPATIBLE!
	*/
	bool GeneratorHelper::checkCompatibilityRuleTypeBefore(NodeVec nodes, Node* S, Node* N, Relation* R, Rule rule, bool isStrict)
	{
		/* */
		const Object* lhsO = rule.getLeftHandSideObject();
		const State* lhsS = rule.getLeftHandSideState();

		const Object* rhsO = rule.getRightHandSideObject();
		const State* rhsS = rule.getRightHandSideState();

		// LHS = N and RHS = S
		// S < N automatically false
		if (GeneratorHelper::isRuleObjectEqual(lhsO, N->getRelatedObject()) && GeneratorHelper::isRuleStateEqual(lhsS, N->getGoalState()) &&
			GeneratorHelper::isRuleObjectEqual(rhsO, S->getRelatedObject()) && GeneratorHelper::isRuleStateEqual(rhsS, S->getGoalState())) {
			Logger::log("LHS = N and RHS = S; FALSE");
			return false;
		}

		// Find all existing Right-Hand-side nodes of rule
		NodeVec existingRHS = R->findNodesByPattern(nodes, rhsO, rhsS, GeneratorHelper::isRuleObjectEqual, GeneratorHelper::isRuleStateEqual);
		NodeVec existingLHS = R->findNodesByPattern(nodes, lhsO, lhsS, GeneratorHelper::isRuleObjectEqual, GeneratorHelper::isRuleStateEqual);

		// LHS = S and RHS = N
		// S < N for all N
		// In case of a strict rule, ONLY S<N is allowed.
		if (isRuleObjectEqual(lhsO, S->getRelatedObject()) && isRuleStateEqual(lhsS, S->getGoalState())) {

			if (isRuleObjectEqual(rhsO, N->getRelatedObject()) && isRuleStateEqual(rhsS, N->getGoalState())) {
				// check if this is the smallest occurance of N? so S is not after any other occurance of N
				for (auto& n: existingRHS) {
					if (R->findPrecedingNode(N, n, false)) {
						Logger::log("LHS = S and RHS = N; There is a smaller RHS found; FALSE");
						return false;
					}
				}
				Logger::log("LHS = S and RHS = N; TRUE");
				return true;
			}
			else if (isStrict) {
				Logger::log("LHS = S and RHS != N; STRICT=true; FALSE");
				return false;
			}

		}


		// Check if S is RHS or has RHS as a preceding node
		for (auto& n: existingRHS) {
			if (R->findPrecedingNode(S, n, true)) {
				// Then N is not allowed to be LHS or to have LHS as a following node
				for (auto& k: existingLHS) {
					if (R->findFollowingNode(N, k, true)) {
						Logger::log("S is RHS or has RHS as a preceding node - N is LHS or has LHS as following node; FALSE");
						return false;
					}
				}
			}
		}

		// Check if S is LHS or has LHS as a preceding node
		for (auto& n: existingLHS) {
			if (R->findPrecedingNode(S, n, true)) {
				// Then N is not allowed to have RHS as a preceding node
				for (auto& k: existingRHS) {
					if (R->findPrecedingNode(N, k, false)) {
						Logger::log("S is LHS or has LHS as a preceding node - N has RHS as preceding node; FALSE");
						return false;
					}
				}
			}
		}

		// Check if N is LHS or has LHS as a following node
		for (auto& n: existingLHS) {
			if (R->findFollowingNode(N, n, true)) {
				// Then S is not allowed to be RHS or have RHS as a preceding node
				for (auto& k: existingRHS) {
					if (R->findPrecedingNode(S, k, true)) {
						Logger::log("N is LHS or has LHS as a following node - S is RHS or has RHS as preceding node; FALSE");
						return false;
					}
				}
			}
		}

		Logger::log("Node is compatible! TRUE");
		return true;

	}


	bool GeneratorHelper::isRuleNodeEqual(Node* N, Object* ruleObject, State* ruleState)
	{
		Object* nodeObject = N->getRelatedObject();
		const State* nodeState = N->getGoalState();

		return isRuleObjectEqual(nodeObject, ruleObject) && isRuleStateEqual(nodeState, ruleState);
	}

	bool GeneratorHelper::isRuleObjectEqual(const Object* o1, const Object* o2) {
		if (o1 == nullptr || o2 == nullptr) return true;
		return (o1->sameTemplateAs(*o2));
	}

	bool GeneratorHelper::isRuleStateEqual(const State* s1, const State* s2) {
		if (*s1 == STATE_ANY || *s2 == STATE_ANY) return true;
		return (s1->getStateName() == s2->getStateName());
	}


}

