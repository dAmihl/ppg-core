#pragma once

#include "Core.h"
#include "Object.h"

namespace PPG
{
	class Event
	{

	public:
		Event(Str name_v, Object* G)
			: eventName{ name_v }, relatedObject{ G }
		{}
	
		Object* getRelatedObject() const { return relatedObject; };
		const Str& getEventName() const { return eventName; };
		bool getIsReversible() const { return isReversible; };
		void setIsReversible(bool newIsReversible) { isReversible = newIsReversible; };

	private:
		Object* relatedObject;
		Str eventName;
		bool isReversible = true;
	};

}

