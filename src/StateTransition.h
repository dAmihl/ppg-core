#pragma once

#include <unordered_map>
#include "PuzzleState.h"

using EVENT_MAP = std::unordered_map<std::string, std::vector<std::pair<PPG::PuzzleState, PPG::PuzzleState>>>;

namespace PPG
{
	class StateTransition
	{
	public:
		EVENT_MAP* getTransitionMap();
		void addTransition(std::string eventName, PuzzleState origState, PuzzleState newState);
		std::string getTextualOutput();
		std::vector<PuzzleState> getReachableStates();
		std::vector<std::pair<PuzzleState, PuzzleState>> findTransitions(const std::string name);

	private:
		EVENT_MAP transitionMap;
	};


}
