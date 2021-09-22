#pragma once

#include <vector>

#ifndef PPG_EXPORT
	#define PPG_EXPORT __declspec(dllexport)
#endif

namespace PPG
{
	template<typename T> using Vec = std::vector<T>;
	template<typename T, typename U> using Pair = std::pair<T, U>;

	template<typename T>
	struct PuzzleList {
		typedef std::vector<T> Type;
	};

}