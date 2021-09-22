#pragma once

#include <cstdlib>
#include "PuzzGenCore.h"
#include "State.h"
#include "Object.h"
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
		static T getRandomFromList(std::vector<T> list)
		{
			if (list.empty()) return nullptr;
			int index = rand() % list.size();

			return list.at(index);
		};

		static Object* getRandomObjectFromList(PuzzleList<Object*>::Type list);
		static State getRandomStateFromList(PuzzleList<State>::Type list);
		static Node* getRandomNodeFromList(PuzzleList<Node*>::Type list);

		static unsigned int getRandomUintFromRange(unsigned int min, unsigned int max);

	};
}



