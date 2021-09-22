#pragma once

#include <unordered_map>
#include "PuzzleState.h"


class StateTransition
{
	using EVENT_MAP = std::unordered_map<std::string, std::vector<std::pair<PuzzleState, PuzzleState>>>;
public:
	StateTransition();
	~StateTransition();
	EVENT_MAP* getTransitionMap();
	void addTransition(std::string eventName, PuzzleState origState, PuzzleState newState);
	std::string getTextualOutput();
	std::vector<PuzzleState> getReachableStates();
	std::vector<std::pair<PuzzleState, PuzzleState>> findTransitions(const std::string name);

private:
	EVENT_MAP transitionMap;
};

