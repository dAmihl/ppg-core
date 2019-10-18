#pragma once

#include <vector>
#include "PuzzleNode.h"

#include "PUZZGEN_TYPES.h"

class PuzzleRelation
{
public:
	PuzzleRelation();
	~PuzzleRelation();

	T_PuzzlePairList getPairs();
	void addPair(PuzzleNode* lhs, PuzzleNode* rhs);
	void addPair(T_PuzzleNodePair pair);

	void removePair(T_PuzzleNodePair pair);

	std::string getSimpleTextualRepresentation();
	std::string getRecursiveTextualRepresentationOfNode(T_PuzzleNodeList &alreadyOut, std::string* out, PuzzleNode* N, int level);
	std::string getExtendedTextualRepresentation(T_PuzzleNodeList nodes);

	void checkDoForAllPreceding(PuzzleNode *N, bool(*Check)(PuzzleNode), void(*Do)(PuzzleNode*));
	void checkDoForAllFollowing(PuzzleNode *N, bool(*Check)(PuzzleNode), void(*Do)(PuzzleNode*));

	bool checkAllLarger(PuzzleNode* N, bool(*Check)(PuzzleNode));
	bool checkAllSmaller(PuzzleNode* N, bool(*Check)(PuzzleNode));

	bool checkAllFollowing(PuzzleNode* N, bool(*Check)(PuzzleNode));
	bool checkAllPreceding(PuzzleNode* N, bool(*Check)(PuzzleNode));
	bool checkAtLeastOneFollowing(PuzzleNode* N, bool(*Check)(PuzzleNode));
	bool checkAtLeastOnePreceding(PuzzleNode* N, bool(*Check)(PuzzleNode));



	T_PuzzlePairList getNextPairs(T_PuzzleNodePair P);
	T_PuzzlePairList getParallelPairs(T_PuzzleNodePair P);

	bool hasPrecedingNode(PuzzleNode* N);
	bool hasFollowingNode(PuzzleNode* N);

	T_PuzzleNodeList getMinima(T_PuzzleNodeList nodes);
	T_PuzzleNodeList getMaxima(T_PuzzleNodeList nodes);

	T_PuzzleNodeList getSmallestNodesFromList(T_PuzzleNodeList nodes);
	T_PuzzleNodeList getLargestNodesFromList(T_PuzzleNodeList nodes);

	bool findPrecedingNode(PuzzleNode* start, PuzzleNode* nodeToFind);
	bool findFollowingNode(PuzzleNode* start, PuzzleNode* nodeToFind);
	bool findPrecedingNode(PuzzleNode* start, PuzzleNode* nodeToFind, bool includeStart);
	bool findFollowingNode(PuzzleNode* start, PuzzleNode* nodeToFind, bool includeStart);

	bool findDirectlyPrecedingNode(PuzzleNode* start, PuzzleNode* nodeToFind);
	bool findDirectlyFollowingNode(PuzzleNode* start, PuzzleNode* nodeToFind);


	T_PuzzleNodeList findNodesByPattern(T_PuzzleNodeList nodes, PuzzleObject* O, PuzzleState* S, bool(*EqualObject)(PuzzleObject*, PuzzleObject*), bool(*EqualState)(PuzzleState*, PuzzleState*));

	T_PuzzleNodeList findNearestPrecedingEqualNodesByObject(PuzzleNode* N);
	T_PuzzleNodeList findNearestPrecedingEqualNodesByObject(PuzzleNode* N, PuzzleNode* start);
	T_PuzzleNodeList findNearestFollowingEqualNodesByObject(PuzzleNode* N);
	T_PuzzleNodeList findNearestFollowingEqualNodesByObject(PuzzleNode* N, PuzzleNode* start);

	T_PuzzleNodeList filterCompatibleNodesByRules(T_PuzzleNodeList nodes, T_PuzzleRuleList rules);
	

	T_PuzzleNodeList getPrecedingNodes(PuzzleNode* N);
	T_PuzzleNodeList getFollowingNodes(PuzzleNode* N);

	static T_PuzzleNodePair makePuzzlePair(PuzzleNode* N1, PuzzleNode* N2);

private:
	T_PuzzlePairList pairs;

};

