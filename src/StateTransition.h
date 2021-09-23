#pragma once

#include "PuzzGenCore.h"
#include <unordered_map>
#include "State.h"

using EVENT_MAP = std::unordered_map<std::string, std::vector<std::pair<PPG::State*, PPG::State*>>>;

namespace PPG
{
	class PPG_EXPORT StateTransition
	{
	public:
		EVENT_MAP* getTransitionMap();
		void addTransition(std::string eventName, State* origState, State* newState);
		std::string getTextualOutput() const;
		std::vector<State*> getReachableStates() const;
		std::vector<std::pair<State*, State*>> findTransitions(const std::string name) const;

	private:
		EVENT_MAP transitionMap;
	};


}
