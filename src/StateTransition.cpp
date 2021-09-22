#include "StateTransition.h"

namespace PPG
{

	EVENT_MAP* StateTransition::getTransitionMap()
	{
		return &this->transitionMap;
	}

	void StateTransition::addTransition(std::string eventName, State origState, State newState)
	{

		bool eventEntryFound = (this->transitionMap.find(eventName) != this->transitionMap.end());
		std::pair<State, State> newStatePair;
		newStatePair.first = origState;
		newStatePair.second = newState;

		if (eventEntryFound) {
			this->transitionMap.find(eventName)->second.push_back(newStatePair);
		}
		else {
			std::vector<std::pair<State, State>> newVector;
			newVector.push_back(newStatePair);
			this->transitionMap.insert(std::make_pair(eventName, newVector));
		}

	}

	std::string StateTransition::getTextualOutput() const
	{
		std::string out = "";
		for (auto& it: transitionMap) {
			out += "> " + it.first + ": ";
			for (auto& vecIt: it.second) {
				out += "<" + vecIt.first.getStateName() + " -> " + vecIt.second.getStateName() + "> ";
			}
			out += "\n";
		}

		return out;
	}

	std::vector<State> StateTransition::getReachableStates() const
	{
		std::vector<State> reachables;
		for (auto& it: transitionMap) {
			for (auto& vecIt : it.second) {
				reachables.push_back(vecIt.second);
			}
		}
		return reachables;
	}

	std::vector<std::pair<State, State>> StateTransition::findTransitions(const std::string name) const
	{
		auto& it = transitionMap.find(name);
		if (it != transitionMap.end()) {
			return it->second;
		}

		std::vector<std::pair<State, State>> a;
		return a;
	}
}
