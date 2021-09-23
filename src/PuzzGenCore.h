#pragma once

#include <vector>
#include <memory>

#ifndef PPG_EXPORT
	//#define PPG_EXPORT __declspec(dllexport)
	#define PPG_EXPORT
#endif

namespace PPG
{
	template<typename T> using Vec = std::vector<T>;
	template<typename T, typename U> using Pair = std::pair<T, U>;
	template<typename T> using UPtr = std::unique_ptr<T>;
	template<typename T> using Ptr = std::shared_ptr<T>;

	template<typename T>
	struct PuzzleList {
		typedef std::vector<T> Type;
	};

}