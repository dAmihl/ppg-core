#pragma once
#include <vector>
#include "PuzzleNode.h"
#include "PuzzleRelation.h"
#include "PuzzleRandomizer.h"
#include "PUZZGEN_TYPES.h"
#include "Puzzle.h"
#include "PuzzleGeneratorHelper.h"

class PuzzleGenerator
{
public:
	PuzzleGenerator();
	~PuzzleGenerator();
	
	Puzzle* generatePuzzle(T_PuzzleObjectList objects, T_PuzzleEventList events, T_PuzzleRuleList rules);

	PuzzleRelation* __simple_generateRelation(T_PuzzleNodeList nodes);

	PuzzleRelation* generateRelation(T_PuzzleNodeList nodes, T_PuzzleRuleList rules);

	T_PuzzleNodeList generateNodes(T_PuzzleObjectList objects);

	void setNumberNodes(int n);
	int getNumberNodes();

private:

	void _initializeActivePropertyOnNodes(Puzzle* P);

	void _cleanupNodes(Puzzle* P);
	void _removeNodeFromList(PuzzleNode*N, T_PuzzleNodeList &nodes);
	int NUM_NODES = 0;
};

