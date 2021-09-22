#pragma once

#include <vector>
#include "PuzzleRule.h"
#include "Node.h"
#include "PuzzleObject.h"
#include "PuzzleEvent.h"
#include "PuzzleState.h"

namespace PPG
{
	template<typename T> using Vec = std::vector<T>;
	template<typename T, typename U> using Pair = std::pair<T, U>;

	template<typename T>
	struct PuzzleList {
		typedef std::vector<T> Type;
	};

}