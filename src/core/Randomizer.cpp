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

	unsigned int Randomizer::getRandomUintFromRange(unsigned int min, unsigned int max)
	{
		unsigned int result = min;
		result += (rand() % (max));
		return result;
	}

}
