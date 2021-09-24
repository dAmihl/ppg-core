#include "StateTransition.h"

namespace PPG
{

	EventMap& StateTransition::getTransitionMap()
{
		return transitionMap;
	}

	void StateTransition::addTransition(Str eventName, State origState, State newState)
	{

		bool eventEntryFound = (transitionMap.find(eventName) != transitionMap.end());
		std::pair<State, State> newStatePair;
		newStatePair.first = origState;
		newStatePair.second = newState;

		if (eventEntryFound) {
			transitionMap.find(eventName)->second.push_back(newStatePair);
		}
		else {
			Vec<Pair<State, State>> newVector;
			newVector.push_back(newStatePair);
			transitionMap.insert(std::make_pair(eventName, newVector));
		}

	}

	Str StateTransition::getTextualOutput() const
	{
		Str out = "";
		for (auto& it: transitionMap) {
			out += "> " + it.first + ": ";
			for (auto& vecIt: it.second) {
				out += "<" + vecIt.first.getName() + " -> " + vecIt.second.getName() + "> ";
			}
			out += "\n";
		}

		return out;
	}

	Vec<State> StateTransition::getReachableStates() const
	{
		Vec<State> reachables;
		for (auto& it: transitionMap) {
			for (auto& vecIt : it.second) {
				reachables.push_back(vecIt.second);
			}
		}
		return reachables;
	}

	EventMapVal StateTransition::findTransitions(const Str name) const
	{
		auto& it = transitionMap.find(name);
		if (it != transitionMap.end()) {
			return it->second;
		}

		EventMapVal a;
		return a;
	}
}
