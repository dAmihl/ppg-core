#pragma once

#include "core/Core.h"
#include "Object.h"

namespace PPG
{
	class Event
	{

	public:
		Event(Str name_v, Ptr<Object> G)
			: eventName{ name_v }, relatedObject{ G }
		{}
	
		Object& getRelatedObject() const { return *relatedObject; };
		const Str& getEventName() const { return eventName; };
		bool getIsReversible() const { return isReversible; };
		void setIsReversible(bool newIsReversible) { isReversible = newIsReversible; };

	private:
		Ptr<Object> relatedObject;
		Str eventName;
		bool isReversible = true;
	};

}

