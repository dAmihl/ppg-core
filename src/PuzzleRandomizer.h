#pragma once

#include <cstdlib>
#include "PUZZGEN_TYPES.h"
#include "PuzzleState.h"

class PuzzleRandomizer
{
public:
	PuzzleRandomizer();
	~PuzzleRandomizer();

	/*
	If no Seed is set, then PuzzleRandomizer will use time(NULL) as default seed
	*/
	static void init();
	static void init(unsigned int seed);
	static PuzzleObject* getRandomObjectFromList(PuzzleList<PuzzleObject*>::Type list);
	static PuzzleState getRandomStateFromList(PuzzleList<PuzzleState>::Type list);
	static PuzzleNode* getRandomNodeFromList(PuzzleList<PuzzleNode*>::Type list);

	static unsigned int getRandomUintFromRange(unsigned int min, unsigned int max);

};


