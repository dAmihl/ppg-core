#pragma once

#include <memory>
#include "PuzzGenCore.h"
#include "Object.h"
#include "Event.h"
#include "State.h"

namespace PPG
{
	template<typename T> using UPtr = std::unique_ptr<T>;

	struct Context
	{

		template<class T, class... _Ts>
		T& add(_Ts&&... params) {
			T* x = new T(params...);
			auto t = std::make_unique<T>(params...);
			return addInt(x);
		}

		Object& addInt(Object* u)
		{
			objects.push_back(std::move(u));
			return *objects.back();
		}

		State& addInt(State* u)
		{
			states.push_back(u);
			return *states.back();
		}

		Event& addInt(Event* u)
		{
			events.push_back(u);
			return *events.back();
		}
		
		Vec<Object*>& getObjects() { return objects; }

	private:
		Vec<Object*> objects;
		Vec<State*> states;
		Vec<Event*> events;

	};
}