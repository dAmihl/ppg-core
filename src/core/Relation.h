#pragma once

#include "Core.h"
#include "GraphNode.h"
#include "Node.h"
#include "../Rule.h"
#include <algorithm>

namespace PPG
{
	using NodePair = Pair<Ptr<Node>, Ptr<Node>>;
	using NodePairVec = Vec<NodePair>;
	using NodeVec = Vec<Ptr<Node>>;

	class PPG_EXPORT Relation
	{
	public:
		NodePairVec getPairs();
		void addPair(Ptr<Node> lhs, Ptr<Node> rhs);
		void addPair(NodePair pair);

		void removePair(NodePair& pair);

		Str getSimpleTextualRepresentation() const;
		Str getRecursiveTextualRepresentationOfNode(Vec<Ptr<Node>>& alreadyOut, Str* out, const Ptr<Node>& N, int level) const;
		Str getExtendedTextualRepresentation(const NodeVec& nodes) const;

		Vec<GraphNode*> getGraphRepresentation(const NodeVec& nodes) const;
		GraphNode* getRecursiveGraphRepresentation(const Ptr<Node>& N) const;

		void checkDoForAllPreceding(const Ptr<Node>& N, bool(*Check)(const Node&), void(*Do)(Node&));
		void checkDoForAllFollowing(const Ptr<Node>& N, bool(*Check)(const Node&), void(*Do)(Node&));

		bool checkAllLarger(const Ptr<Node>& N, bool(*Check)(const Node&)) const;
		bool checkAllSmaller(const Ptr<Node>& N, bool(*Check)(const Node&)) const;

		bool checkAllFollowing(const Ptr<Node>& N, bool(*Check)(const Node&)) const;
		bool checkAllPreceding(const Ptr<Node>& N, bool(*Check)(const Node&)) const;
		bool checkAtLeastOneFollowing(const Ptr<Node>& N, bool(*Check)(const Node&)) const;
		bool checkAtLeastOnePreceding(const Ptr<Node>& N, bool(*Check)(const Node&)) const;

		NodePairVec getNextPairs(const NodePair& P) const;
		NodePairVec getParallelPairs(const NodePair& P) const;

		bool hasPrecedingNode(const Ptr<Node>& N) const;
		bool hasFollowingNode(const Ptr<Node>& N) const;

		NodeVec getMinima(const NodeVec& nodes) const;
		NodeVec getMaxima(const NodeVec& nodes) const;

		NodeVec getSmallestNodesFromList(const NodeVec& nodes) const;
		NodeVec getLargestNodesFromList(const NodeVec& nodes) const;

		bool findPrecedingNode(const Ptr<Node> start, const Ptr<Node> nodeToFind) const;
		bool findFollowingNode(const Ptr<Node> start, const Ptr<Node> nodeToFind) const;

		bool findPrecedingNode(const Ptr<Node> start, const Ptr<Node> nodeToFind, bool includeStart) const;
		bool findFollowingNode(const Ptr<Node> start, const Ptr<Node> nodeToFind, bool includeStart) const;

		bool findDirectlyPrecedingNode(const Ptr<Node> start, const Ptr<Node> nodeToFind) const;
		bool findDirectlyFollowingNode(const Ptr<Node> start, const Ptr<Node> nodeToFind) const;

		NodeVec findNearestPrecedingEqualNodesByObject(const Ptr<Node>& N) const;
		NodeVec findNearestPrecedingEqualNodesByObject(const Ptr<Node>& N, const Ptr<Node>& start) const;
		NodeVec findNearestFollowingEqualNodesByObject(const Ptr<Node>& N) const;
		NodeVec findNearestFollowingEqualNodesByObject(const Ptr<Node>& N, const Ptr<Node>& start) const;

		NodeVec getPrecedingNodes(const Ptr<Node>& N) const;
		NodeVec getFollowingNodes(const Ptr<Node>& N) const;

	private:
		NodePairVec pairs;

	};
}

