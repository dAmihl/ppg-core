#pragma once

#include "Core.h"

namespace PPG
{
	class EventManager
	{
	public:

		virtual void onObjectEvent(Str objName, Str eventName) = 0;

	};
}
