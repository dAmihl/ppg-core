#include "StateTransition.h"


StateTransition::StateTransition()
{
}


StateTransition::~StateTransition()
{
}

EVENT_MAP* StateTransition::getTransitionMap()
{
	return &this->transitionMap;
}

void StateTransition::addTransition(std::string eventName, PuzzleState origState, PuzzleState newState)
{
	
	bool eventEntryFound = (this->transitionMap.find(eventName) != this->transitionMap.end());
	std::pair<PuzzleState, PuzzleState> newStatePair;
	newStatePair.first = origState;
	newStatePair.second = newState;

	if (eventEntryFound) {
		this->transitionMap.find(eventName)->second.push_back(newStatePair);
	}
	else {
		std::vector<std::pair<PuzzleState, PuzzleState>> newVector;
		newVector.push_back(newStatePair);
		this->transitionMap.insert(std::make_pair(eventName, newVector));
	}
	
}

std::string StateTransition::getTextualOutput()
{
	std::string out = "";
	for (EVENT_MAP::iterator it = this->transitionMap.begin(); it != this->transitionMap.end(); ++it) {
		out += "> " + it->first + ": ";
		for (std::vector<std::pair<PuzzleState, PuzzleState>>::iterator vecIt = it->second.begin(); vecIt != it->second.end(); ++vecIt) {
			out += "<" + vecIt->first.getStateName() + " -> " + vecIt->second.getStateName() + "> ";
		}
		out += "\n";
	}

	return out;
}

std::vector<PuzzleState> StateTransition::getReachableStates()
{
	std::vector<PuzzleState> reachables;
	for (EVENT_MAP::iterator it = this->transitionMap.begin(); it != this->transitionMap.end(); ++it) {
		for (std::vector<std::pair<PuzzleState, PuzzleState>>::iterator vecIt = it->second.begin(); vecIt != it->second.end(); ++vecIt) {
			reachables.push_back(vecIt->second);
		}
	}
	return reachables;
}

std::vector<std::pair<PuzzleState, PuzzleState>> StateTransition::findTransitions(const std::string name)
{
	EVENT_MAP::iterator it = this->transitionMap.find(name);
	if (it != this->transitionMap.end()) {
		return it->second;
	}

	std::vector<std::pair<PuzzleState, PuzzleState>> a;
	return a;
}
