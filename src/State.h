#pragma once

#include "core/Core.h"

namespace PPG
{
	class State
	{

	public:
		State() : m_name{ "DefaultState" } {};
		State(Str name) : m_name{ name } {};
	
		Str getName() const {
			return m_name;
		}

	private:
		Str m_name;
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