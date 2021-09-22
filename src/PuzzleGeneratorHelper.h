#pragma once

#include "PuzzleRelation.h"
#include "PuzzleRandomizer.h"
#include "PuzzGenCore.h"
#include "Puzzle.h"
#include "PuzzleLogger.h"
#include "PuzzleRule.h"

namespace PPG
{

	using NodePair = Pair<Node*, Node*>;
	using NodePairVec = Vec<NodePair>;
	using NodeVec = Vec<Node*>;
	using RuleVec = Vec<PuzzleRule>;

	class PuzzleGeneratorHelper
	{

	public:
		static bool checkCreatesCircularDependency(NodePair P, PuzzleRelation* R);
		static bool checkCreatesExclusiveDependency(NodePair P, PuzzleRelation* R);
		static bool findNodeSequential(Node* N, NodePair start, PuzzleRelation* R);
		static bool checkEquality(Node* N1, Node* N2);
		static bool checkMetaEqualOccurance(NodePair P, PuzzleRelation* R);


		static bool checkCompatibilityBasicRules(Node* S, Node* N, PuzzleRelation* R);
		static bool checkCompatibilityCustomRules(NodeVec nodes, Node* S, Node* N, PuzzleRelation* R, RuleVec rules);

		static bool checkCompatibilityRuleTypeAfter(NodeVec nodes, Node* S, Node* N, PuzzleRelation* R, PuzzleRule rule, bool isStrict);
		static bool checkCompatibilityRuleTypeBefore(NodeVec nodes, Node* S, Node* N, PuzzleRelation* R, PuzzleRule rule, bool isStrict);

		static NodeVec filterCompatibleNodes(Node* N, PuzzleRelation* R, NodeVec nodes, RuleVec rules);

		static bool checkMetaEqualOccuranceByNode(Node* N, PuzzleRelation* R);

		static bool isRuleNodeEqual(Node* N, PuzzleObject* ruleObject, PuzzleState* ruleState);
		static bool isRuleObjectEqual(PuzzleObject* o1, PuzzleObject* o2);
		static bool isRuleStateEqual(PuzzleState* s1, PuzzleState* s2);

	};

}
