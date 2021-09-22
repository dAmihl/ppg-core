#pragma once
#include <vector>
#include "PuzzleNode.h"
#include "PuzzleRelation.h"
#include "PuzzleRandomizer.h"
#include "PUZZGEN_TYPES.h"
#include "Puzzle.h"
#include "PuzzleGeneratorHelper.h"
#include "PuzzleLogger.h"

class PuzzleGenerator
{
public:

	Puzzle* generatePuzzle(T_PuzzleObjectList objects, T_PuzzleEventList events, T_PuzzleRuleList rules);

	PuzzleRelation* simpleGenerateRelation(T_PuzzleNodeList nodes);

	PuzzleRelation* generateRelation(T_PuzzleNodeList nodes, T_PuzzleRuleList rules);
	PuzzleRelation* generateRelationExperimental(Puzzle* P, T_PuzzleNodeList nodes, T_PuzzleRuleList rules);

	T_PuzzleNodeList generateNodes(T_PuzzleObjectList objects);

	void setNumberNodes(int n);
	int getNumberNodes();

	void setSeed(unsigned int seed);
	unsigned int getSeed();

private:

	void initializeActivePropertyOnNodes(Puzzle* P);

	void cleanupNodes(Puzzle* P);
	void removeNodeFromList(PuzzleNode*N, T_PuzzleNodeList &nodes);
	int NUM_NODES = 0;
	unsigned int seed = 0;
	bool seedSet = false;
};

