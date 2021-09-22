#pragma once

#include "PuzzleRelation.h"
#include "PuzzleRandomizer.h"
#include "PUZZGEN_TYPES.h"
#include "Puzzle.h"
#include "PuzzleLogger.h"
#include "PuzzleRule.h"

class PuzzleGeneratorHelper
{

public: 
	static bool checkCreatesCircularDependency(T_PuzzleNodePair P, PuzzleRelation* R);
	static bool checkCreatesExclusiveDependency(T_PuzzleNodePair P, PuzzleRelation* R);
	static bool findNodeSequential(PuzzleNode* N, T_PuzzleNodePair start, PuzzleRelation* R);
	static bool checkEquality(PuzzleNode* N1, PuzzleNode* N2);
	static bool checkMetaEqualOccurance(T_PuzzleNodePair P, PuzzleRelation* R);


	static bool checkCompatibilityBasicRules(PuzzleNode* S, PuzzleNode* N, PuzzleRelation* R);
	static bool checkCompatibilityCustomRules(T_PuzzleNodeList nodes, PuzzleNode* S, PuzzleNode* N, PuzzleRelation* R, T_PuzzleRuleList rules);

	static bool checkCompatibilityRuleTypeAfter(T_PuzzleNodeList nodes, PuzzleNode* S, PuzzleNode* N, PuzzleRelation* R, PuzzleRule rule, bool isStrict);
	static bool checkCompatibilityRuleTypeBefore(T_PuzzleNodeList nodes, PuzzleNode* S, PuzzleNode* N, PuzzleRelation* R, PuzzleRule rule, bool isStrict);

	static T_PuzzleNodeList filterCompatibleNodes(PuzzleNode* N, PuzzleRelation* R, T_PuzzleNodeList nodes, T_PuzzleRuleList rules);

	static bool checkMetaEqualOccuranceByNode(PuzzleNode* N, PuzzleRelation* R);

	static bool isRuleNodeEqual(PuzzleNode * N, PuzzleObject* ruleObject, PuzzleState* ruleState);
	static bool isRuleObjectEqual(PuzzleObject* o1, PuzzleObject* o2);
	static bool isRuleStateEqual(PuzzleState* s1, PuzzleState* s2);

};
