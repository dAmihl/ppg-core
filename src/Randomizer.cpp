#include "Randomizer.h"
#include <time.h>
#include <random>

namespace PPG
{
	void Randomizer::init()
	{
		srand((unsigned int)time(NULL));
	}

	void Randomizer::init(unsigned int seed)
	{
		srand(seed);
	}

	Object* Randomizer::getRandomObjectFromList(PuzzleList<Object*>::Type list)
	{
		if (list.empty()) return nullptr;
		int index = rand() % list.size();

		return list.at(index);
	}

	State*  Randomizer::getRandomStateFromList(PuzzleList<State*>::Type list)
	{
		if (list.empty()) throw - 1;
		int index = rand() % list.size();

		return (list.at(index));
	}

	Node* Randomizer::getRandomNodeFromList(PuzzleList<Node*>::Type list)
	{
		if (list.empty()) return nullptr;
		int index = rand() % list.size();

		return (list.at(index));
	}

	unsigned int Randomizer::getRandomUintFromRange(unsigned int min, unsigned int max)
	{
		unsigned int result = min;
		result += (rand() % (max));
		return result;
	}

}
