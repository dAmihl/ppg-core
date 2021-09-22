#pragma once

#include <vector>
#include "Rule.h"
#include "Node.h"
#include "Object.h"
#include "Event.h"
#include "State.h"

namespace PPG
{
	template<typename T> using Vec = std::vector<T>;
	template<typename T, typename U> using Pair = std::pair<T, U>;

	template<typename T>
	struct PuzzleList {
		typedef std::vector<T> Type;
	};

}