#pragma once

#include <string>

namespace PPG
{
	class PuzzleState
	{
	public:
		PuzzleState() : m_stateName{ "DefaultState" } {};
		PuzzleState(std::string name) : m_stateName{ name } {};
		std::string getStateName() {
			return m_stateName;
		}

	private:
		std::string m_stateName;
	};

}