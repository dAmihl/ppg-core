#pragma once

#include "core/Core.h"
#include "Object.h"
#include "Event.h"
#include "State.h"

namespace PPG
{

	class Context
	{

	public:

		Context() = default;
		Context(const Context& other) = default;
		Context(Context&& other) noexcept = default;
		Context& operator=(const Context& other) = default;
		Context& operator=(Context&& other) = default;

		~Context() = default;

	public:
		template<class T, class... _Ts>
		Ptr<T> add(_Ts&&... params) {
			auto t = std::make_shared<T>(params...);
			return addInt(std::move(t));
		}

		template<class... _Ts>
		Ptr<Rule> add(_Ts&&... params) = delete;
		template<class... _Ts>
		Ptr<State> add(_Ts&&... params) = delete;

		template<class... _Ts>
		Rule& addRule(_Ts&&... params)
		{
			rules.emplace_back(std::forward<_Ts>(params)...);
			return rules.back();
		}
		
		inline Vec<Ptr<Object>>& getObjects() { return objects; }
		inline Vec<Ptr<Event>>& getEvents() { return events; }
		inline Vec<Rule>& getRules() { return rules; }

	private:
		Ptr<Object> addInt(Ptr<Object>&& u)
		{
			objects.push_back(std::move(u));
			return objects.back();
		}

		Ptr<Event> addInt(Ptr<Event>&& u)
		{
			events.push_back(std::move(u));
			return events.back();
		}
		
	private:
		Vec<Ptr<Object>> objects;
		Vec<Ptr<Event>> events;
		Vec<Rule> rules;

	};
}