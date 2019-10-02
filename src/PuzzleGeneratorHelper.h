#pragma once

#include "PuzzleRelation.h"
#include "PuzzleRandomizer.h"
#include "PUZZGEN_TYPES.h"
#include "Puzzle.h"

class PuzzleGeneratorHelper
{

public: 
	static bool _checkCreatesCircularDependency(T_PuzzleNodePair P, PuzzleRelation* R);
	static bool _checkCreatesExclusiveDependency(T_PuzzleNodePair P, PuzzleRelation* R);
	static bool _findNodeSequential(PuzzleNode* N, T_PuzzleNodePair start, PuzzleRelation* R);
	static bool _checkEquality(PuzzleNode* N1, PuzzleNode* N2);
	static bool _checkMetaEqualOccurance(T_PuzzleNodePair P, PuzzleRelation* R);


	static bool _checkCompatibilityBasicRules(PuzzleNode* S, PuzzleNode* N, PuzzleRelation* R);
	static bool _checkCompatibilityCustomRules(T_PuzzleNodeList nodes, PuzzleNode* S, PuzzleNode* N, PuzzleRelation* R, T_PuzzleRuleList rules);

	static bool _checkCompatibilityRuleType__AFTER(T_PuzzleNodeList nodes, PuzzleNode* S, PuzzleNode* N, PuzzleRelation* R, PuzzleRule rule, bool isStrict);
	static bool _checkCompatibilityRuleType__BEFORE(T_PuzzleNodeList nodes, PuzzleNode* S, PuzzleNode* N, PuzzleRelation* R, PuzzleRule rule, bool isStrict);

	static T_PuzzleNodeList _filterCompatibleNodes(PuzzleNode* N, PuzzleRelation* R, T_PuzzleNodeList nodes, T_PuzzleRuleList rules);

	static bool checkMetaEqualOccuranceByNode(PuzzleNode* N, PuzzleRelation* R);

	static bool __isRuleObjectEqual(PuzzleObject* o1, PuzzleObject* o2);
	static bool __isRuleStateEqual(PuzzleState* s1, PuzzleState* s2);

};
