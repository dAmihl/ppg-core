#pragma once

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

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
	template<typename K, typename V> using Map = std::unordered_map<K, V>;
	
	using Str = std::string;

	template<typename T>
	struct PuzzleList {
		typedef Vec<T> Type;
	};


}