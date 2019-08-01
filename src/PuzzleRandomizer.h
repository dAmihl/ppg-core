#pragma once

#include <cstdlib>
#include "PUZZGEN_TYPES.h"

class PuzzleRandomizer
{
public:
	PuzzleRandomizer();
	~PuzzleRandomizer();

	static void init();
	static PuzzleObject* getRandomObjectFromList(PuzzleList<PuzzleObject*>::Type list);
	static PuzzleState getRandomStateFromList(PuzzleList<PuzzleState>::Type list);
	static PuzzleNode* getRandomNodeFromList(PuzzleList<PuzzleNode*>::Type list);

	static unsigned int getRandomUintFromRange(unsigned int min, unsigned int max);

};


