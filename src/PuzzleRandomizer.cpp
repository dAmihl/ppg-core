#include "PuzzleRandomizer.h"
#include <time.h>
#include <random>

namespace PPG
{
	void PuzzleRandomizer::init()
	{
		srand((unsigned int)time(NULL));
	}

	void PuzzleRandomizer::init(unsigned int seed)
	{
		srand(seed);
	}

	PuzzleObject* PuzzleRandomizer::getRandomObjectFromList(PuzzleList<PuzzleObject*>::Type list)
	{
		if (list.empty()) return nullptr;
		int index = rand() % list.size();

		return list.at(index);
	}

	PuzzleState  PuzzleRandomizer::getRandomStateFromList(PuzzleList<PuzzleState>::Type list)
	{
		if (list.empty()) throw - 1;
		int index = rand() % list.size();

		return (list.at(index));
	}

	Node* PuzzleRandomizer::getRandomNodeFromList(PuzzleList<Node*>::Type list)
	{
		if (list.empty()) return nullptr;
		int index = rand() % list.size();

		return (list.at(index));
	}

	unsigned int PuzzleRandomizer::getRandomUintFromRange(unsigned int min, unsigned int max)
	{
		unsigned int result = min;
		result += (rand() % (max));
		return result;
	}

}
