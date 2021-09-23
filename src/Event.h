#pragma once

#include "PuzzGenCore.h"
#include "Object.h"
#include <string>

namespace PPG
{
	class Event
	{

	public:
		Event(std::string name_v, Object* G)
			: eventName{ name_v }, relatedObject{ G }
		{}
	
		Object* getRelatedObject() const { return relatedObject; };
		std::string getEventName() const { return eventName; };
		bool getIsReversible() const { return isReversible; };
		void setIsReversible(bool newIsReversible) { isReversible = newIsReversible; };

	private:
		Object* relatedObject;
		std::string eventName;
		bool isReversible = true;
	};

}

