#pragma once

#include "core/Core.h"

namespace PPG
{
	class State
	{

	public:
		explicit State() : m_name{ "DefaultState" } {};
		explicit State(const Str name) : m_name{ name } {};

		State(const State& other) : m_name{ other.m_name }
		{}

		const Str getName() const {
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