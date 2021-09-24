#pragma once

#include "Core.h"
#include "Object.h"
#include "Event.h"
#include "State.h"

namespace PPG
{

	class Context
	{

	public:
		template<class T, class... _Ts>
		Ptr<T> add(_Ts&&... params) {
			auto t = std::make_shared<T>(params...);
			return addInt(std::move(t));
		}
		
		inline Vec<Ptr<Object>>& getObjects() { return objects; }
		inline Vec<Ptr<Event>>& getEvents() { return events; }
		inline Vec<Ptr<Rule>>& getRules() { return rules; }

	private:
		Ptr<Object>& addInt(Ptr<Object>&& u)
		{
			objects.push_back(std::move(u));
			return objects.back();
		}


		Ptr<Event>& addInt(Ptr<Event>&& u)
		{
			events.push_back(std::move(u));
			return events.back();
		}

		Ptr<Rule>& addInt(Ptr<Rule>&& u)
		{
			rules.push_back(std::move(u));
			return rules.back();
		}
		
	private:
		Vec<Ptr<Object>> objects;
		Vec<Ptr<Event>> events;
		Vec<Ptr<Rule>> rules;

	};
}