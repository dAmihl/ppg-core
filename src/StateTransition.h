#pragma once

#include "PuzzGenCore.h"
#include <unordered_map>
#include "State.h"

using EventMapVal = std::vector<std::pair<PPG::State, PPG::State>>;
using EventMap = std::unordered_map<std::string, EventMapVal>;


namespace PPG
{
	class PPG_EXPORT StateTransition
	{
	public:
		EventMap& getTransitionMap();
		void addTransition(std::string eventName, State origState, State newState);
		std::string getTextualOutput() const;
		Vec<State> getReachableStates() const;
		EventMapVal findTransitions(const std::string name) const;

	private:
		EventMap transitionMap;
	};


}
