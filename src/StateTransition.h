#pragma once

#include "core/Core.h"
#include "State.h"

namespace PPG
{
	using EventMapVal = Vec<Pair<State, State>>;
	using EventMap = Map<Str, EventMapVal>;

	class PPG_EXPORT StateTransition
	{
	public:
		EventMap& getTransitionMap();
		void addTransition(Str eventName, State origState, State newState);
		Str getTextualOutput() const;
		Vec<State> getReachableStates() const;
		EventMapVal findTransitions(const Str name) const;

	private:
		EventMap transitionMap;
	};


}
