#pragma once

#include "PuzzGenCore.h"
#include <string>

namespace PPG
{
	class State
	{

	public:
		State() : m_name{ "DefaultState" } {};
		State(std::string name) : m_name{ name } {};
	
		std::string getName() const {
			return m_name;
		}

	private:
		std::string m_name;
	};

	inline bool operator==(const State& lhs, const State& rhs)
	{
		return lhs.getName() == rhs.getName();
	}

	inline bool operator!=(const State& lhs, const State& rhs)
	{
		return !(lhs.getName() == rhs.getName());
	}

	static const State STATE_ANY{ "STATE_ANY" };
}