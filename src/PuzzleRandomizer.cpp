#include "PuzzleRandomizer.h"
#include <time.h>
#include <random>


void PuzzleRandomizer::init()
{
	srand((unsigned int)time(NULL));
}

void PuzzleRandomizer::init(unsigned int seed)
{
	srand( seed );
}

template<typename T>
T PuzzleRandomizer::getRandomFromList(std::vector<T> list)
{
	if (list.empty()) return nullptr;
	int index = rand() % list.size();

	return list.at(index);
}

PuzzleObject* PuzzleRandomizer::getRandomObjectFromList(PuzzleList<PuzzleObject*>::Type list)
{
	if (list.empty()) return nullptr;
	int index = rand() % list.size();

	return list.at(index);
}

PuzzleState  PuzzleRandomizer::getRandomStateFromList(PuzzleList<PuzzleState>::Type list)
{
	if (list.empty()) throw -1;
	int index = rand() % list.size();

	return (list.at(index));
}

PuzzleNode * PuzzleRandomizer::getRandomNodeFromList(PuzzleList<PuzzleNode*>::Type list)
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
