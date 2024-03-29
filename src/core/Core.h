#pragma once

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include <set>
#include <array>

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
	template<typename T> using WPtr = std::weak_ptr<T>;
	template<typename K, typename V> using Map = std::unordered_map<K, V>;
	template<typename T> using Set = std::set<T>;
	
	using Str = std::string;

	template<class T, class... _Ts>
	Ptr<T> make(_Ts&&... params) {
		auto t = std::make_shared<T>(params...);
		return t;
	}

	template<class T, class... _Ts>
	UPtr<T> makeU(_Ts&&... params) {
		auto t = std::make_unique<T>(params...);
		return t;
	}

	template<class T, class U>
	Pair<T, U> makePair(T a, U b)
	{
		return std::make_pair(a, b);
	}

}