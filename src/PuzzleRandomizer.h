#pragma once

#include <cstdlib>
#include "PuzzGenCore.h"
#include "PuzzleState.h"

namespace PPG {

	class PuzzleRandomizer
	{
	public:
		/*
		If no Seed is set, then PuzzleRandomizer will use time(NULL) as default seed
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

		static PuzzleObject* getRandomObjectFromList(PuzzleList<PuzzleObject*>::Type list);
		static PuzzleState getRandomStateFromList(PuzzleList<PuzzleState>::Type list);
		static Node* getRandomNodeFromList(PuzzleList<Node*>::Type list);

		static unsigned int getRandomUintFromRange(unsigned int min, unsigned int max);

	};
}



