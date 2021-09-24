#pragma once

#include "Core.h"
#include "Relation.h"
#include "Randomizer.h"
#include "Logger.h"

namespace PPG
{

	using NodePair = Pair<Node*, Node*>;
	using NodePairVec = Vec<NodePair>;
	using NodeVec = Vec<Node*>;
	using RuleVec = Vec<Rule>;

	class PPG_EXPORT GeneratorHelper
	{

	public:
		static bool checkCreatesCircularDependency(NodePair P, Relation& R);
		static bool checkCreatesExclusiveDependency(NodePair P, Relation& R);
		static bool findNodeSequential(Node* N, NodePair start, Relation& R);
		static bool checkEquality(Node* N1, Node* N2);
		static bool checkMetaEqualOccurance(NodePair P, Relation& R);


		static bool checkCompatibilityBasicRules(Node* S, Node* N, Relation& R);
		static bool checkCompatibilityCustomRules(NodeVec nodes, Node* S, Node* N, Relation& R, RuleVec& rules);

		static bool checkCompatibilityRuleTypeAfter(NodeVec nodes, Node* S, Node* N, Relation& R, const Rule& rule, bool isStrict);
		static bool checkCompatibilityRuleTypeBefore(NodeVec nodes, Node* S, Node* N, Relation& R, const Rule& rule, bool isStrict);

		static NodeVec filterCompatibleNodes(Node* N, Relation& R, NodeVec nodes, RuleVec& rules);

		static bool checkMetaEqualOccuranceByNode(Node* N, Relation& R);

		static bool isRuleNodeEqual(Node* N, const Object& ruleObject, State ruleState);
		static bool isRuleObjectEqual(const Object& o1, const Object& o2);
		static bool isRuleStateEqual(const State s1, const State s2);

	};

}
