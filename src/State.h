#pragma once

#include "PuzzGenCore.h"
#include <string>

namespace PPG
{
	class State
	{
	public:
		State() : m_stateName{ "DefaultState" } {};
		State(std::string name) : m_stateName{ name } {};
		std::string getStateName() const {
			return m_stateName;
		}

	private:
		std::string m_stateName;
	};

	inline bool operator==(const State& lhs, const State& rhs)
	{
		return lhs.getStateName() == rhs.getStateName();
	}

	static const State STATE_ANY{ "STATE_ANY" };
}