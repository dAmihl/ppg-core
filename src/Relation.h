#pragma once

#include <vector>
#include "PuzzGenCore.h"
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

		std::string getSimpleTextualRepresentation();
		std::string getRecursiveTextualRepresentationOfNode(NodeVec& alreadyOut, std::string* out, Node* N, int level);
		std::string getExtendedTextualRepresentation(NodeVec nodes);

		Vec<GraphNode*> getGraphRepresentation(NodeVec nodes);
		GraphNode* getRecursiveGraphRepresentation(Node* N);

		void checkDoForAllPreceding(Node* N, bool(*Check)(Node), void(*Do)(Node*));
		void checkDoForAllFollowing(Node* N, bool(*Check)(Node), void(*Do)(Node*));

		bool checkAllLarger(Node* N, bool(*Check)(Node));
		bool checkAllSmaller(Node* N, bool(*Check)(Node));

		bool checkAllFollowing(Node* N, bool(*Check)(Node));
		bool checkAllPreceding(Node* N, bool(*Check)(Node));
		bool checkAtLeastOneFollowing(Node* N, bool(*Check)(Node));
		bool checkAtLeastOnePreceding(Node* N, bool(*Check)(Node));



		NodePairVec getNextPairs(NodePair P);
		NodePairVec getParallelPairs(NodePair P);

		bool hasPrecedingNode(Node* N);
		bool hasFollowingNode(Node* N);

		NodeVec getMinima(NodeVec nodes);
		NodeVec getMaxima(NodeVec nodes);

		NodeVec getSmallestNodesFromList(NodeVec nodes);
		NodeVec getLargestNodesFromList(NodeVec nodes);

		bool findPrecedingNode(Node* start, Node* nodeToFind);
		bool findFollowingNode(Node* start, Node* nodeToFind);
		bool findPrecedingNode(Node* start, Node* nodeToFind, bool includeStart);
		bool findFollowingNode(Node* start, Node* nodeToFind, bool includeStart);

		bool findDirectlyPrecedingNode(Node* start, Node* nodeToFind);
		bool findDirectlyFollowingNode(Node* start, Node* nodeToFind);


		NodeVec findNodesByPattern(NodeVec nodes, Object* O, State* S, bool(*EqualObject)(Object*, Object*), bool(*EqualState)(State*, State*));

		NodeVec findNearestPrecedingEqualNodesByObject(Node* N);
		NodeVec findNearestPrecedingEqualNodesByObject(Node* N, Node* start);
		NodeVec findNearestFollowingEqualNodesByObject(Node* N);
		NodeVec findNearestFollowingEqualNodesByObject(Node* N, Node* start);

		NodeVec filterCompatibleNodesByRules(NodeVec nodes, Vec<Rule> rules);


		NodeVec getPrecedingNodes(Node* N);
		NodeVec getFollowingNodes(Node* N);

		static NodePair makePuzzlePair(Node* N1, Node* N2);

	private:
		NodePairVec pairs;

	};
}

