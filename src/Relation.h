#pragma once

#include "Core.h"
#include "GraphNode.h"
#include "Node.h"
#include "Rule.h"

namespace PPG
{
	using NodePair = Pair<Node*, Node*>;
	using NodePairVec = Vec<NodePair>;
	using NodeVec = Vec<Node*>;

	class PPG_EXPORT Relation
	{
	public:
		NodePairVec getPairs();
		void addPair(Node* lhs, Node* rhs);
		void addPair(NodePair pair);

		void removePair(NodePair pair);

		Str getSimpleTextualRepresentation() const;
		Str getRecursiveTextualRepresentationOfNode(Vec<const Node*>& alreadyOut, Str* out, const Node* N, int level) const;
		Str getExtendedTextualRepresentation(const NodeVec nodes) const;

		Vec<GraphNode*> getGraphRepresentation(const NodeVec nodes) const;
		GraphNode* getRecursiveGraphRepresentation(const Node* N) const;

		void checkDoForAllPreceding(const Node* N, bool(*Check)(const Node&), void(*Do)(Node&));
		void checkDoForAllFollowing(const Node* N, bool(*Check)(const Node&), void(*Do)(Node&));

		bool checkAllLarger(const Node* N, bool(*Check)(const Node&)) const;
		bool checkAllSmaller(const Node* N, bool(*Check)(const Node&)) const;

		bool checkAllFollowing(const Node* N, bool(*Check)(const Node&)) const;
		bool checkAllPreceding(const Node* N, bool(*Check)(const Node&)) const;
		bool checkAtLeastOneFollowing(const Node* N, bool(*Check)(const Node&)) const;
		bool checkAtLeastOnePreceding(const Node* N, bool(*Check)(const Node&)) const;



		NodePairVec getNextPairs(const NodePair P) const;
		NodePairVec getParallelPairs(const NodePair P) const;

		bool hasPrecedingNode(const Node* N) const;
		bool hasFollowingNode(const Node* N) const;

		NodeVec getMinima(const NodeVec nodes) const;
		NodeVec getMaxima(const NodeVec nodes) const;

		NodeVec getSmallestNodesFromList(const NodeVec nodes) const;
		NodeVec getLargestNodesFromList(const NodeVec nodes) const;

		bool findPrecedingNode(const Node* start, const Node* nodeToFind) const;
		bool findFollowingNode(const Node* start, const Node* nodeToFind) const;
		bool findPrecedingNode(const Node* start, const Node* nodeToFind, bool includeStart) const;
		bool findFollowingNode(const Node* start, const Node* nodeToFind, bool includeStart) const;

		bool findDirectlyPrecedingNode(const Node* start, const Node* nodeToFind) const;
		bool findDirectlyFollowingNode(const Node* start, const Node* nodeToFind) const;

		NodeVec findNodesByPattern(const NodeVec nodes, const Ptr<Object> O, const State S, bool(*EqualObject)(const Ptr<Object>, const Ptr<Object>), bool(*EqualState)(const State, const State)) const;

		NodeVec findNearestPrecedingEqualNodesByObject(const Node* N) const;
		NodeVec findNearestPrecedingEqualNodesByObject(const Node* N, const Node* start) const;
		NodeVec findNearestFollowingEqualNodesByObject(const Node* N) const;
		NodeVec findNearestFollowingEqualNodesByObject(const Node* N, const Node* start) const;

		NodeVec filterCompatibleNodesByRules(const NodeVec nodes, const Vec<Rule> rules) const;

		NodeVec getPrecedingNodes(const Node* N) const;
		NodeVec getFollowingNodes(const Node* N) const;

		static NodePair makePuzzlePair(Node* N1, Node* N2);

	private:
		NodePairVec pairs;

	};
}

