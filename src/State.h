#pragma once

#include <string>

namespace PPG
{
	class State
	{
	public:
		State() : m_stateName{ "DefaultState" } {};
		State(std::string name) : m_stateName{ name } {};
		std::string getStateName() {
			return m_stateName;
		}

	private:
		std::string m_stateName;
	};

}