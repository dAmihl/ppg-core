#pragma once

#include <string>

namespace PPG
{
	class EventManager
	{
	public:

		virtual void onObjectEvent(std::string objName, std::string eventName) = 0;

	};
}
