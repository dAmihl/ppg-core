#pragma once

#include <cstdlib>
#include "Core.h"
#include "../State.h"
#include "../Object.h"
#include "Node.h"

namespace PPG {

	class PPG_EXPORT Randomizer
	{
	public:
		/*
		If no Seed is set, then Randomizer will use time(NULL) as default seed
		*/
		static void init();
		static void init(unsigned int seed);

		template<typename T>
		static T getRandomFromList(const Vec<T>& list)
		{
			if (list.empty()) return nullptr;
			int index = rand() % list.size();
			return list.at(index);
		};


		static State getRandomFromList(const Vec<State>& list)
		{
			if (list.empty()) return STATE_ANY;
			int index = rand() % list.size();
			return list.at(index);
		}

		static unsigned int getRandomUintFromRange(unsigned int min, unsigned int max);

	};
}



