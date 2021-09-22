#pragma once

#include <string>

namespace PPG
{
	class PuzzleEventManager
	{
	public:

		virtual void onObjectEvent(std::string objName, std::string eventName) = 0;

	};
}
