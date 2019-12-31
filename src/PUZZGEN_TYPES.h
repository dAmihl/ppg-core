#pragma once

#include <vector>
#include "PuzzleRule.h"


class PuzzleNode;
class PuzzleObject;
class PuzzleState;
class PuzzleGraphNode;
class PuzzleEvent;

typedef std::pair<PuzzleNode*, PuzzleNode*> T_PuzzleNodePair;

typedef std::vector<T_PuzzleNodePair> T_PuzzlePairList;
typedef std::vector<PuzzleNode*> T_PuzzleNodeList;
typedef std::vector<PuzzleObject*> T_PuzzleObjectList;
typedef std::vector<PuzzleEvent*> T_PuzzleEventList;
typedef std::vector<PuzzleRule> T_PuzzleRuleList;
typedef std::vector<PuzzleGraphNode*> T_PuzzleGraphNodeList;

typedef std::pair<PuzzleObject*, PuzzleState*> T_PuzzleObjectStatePair;

template<typename T>
struct PuzzleList {
	typedef std::vector<T> Type;
};