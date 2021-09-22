#pragma once

#include <string>

class PuzzleEventManager
{
public:

	virtual void onObjectEvent(std::string objName, std::string eventName) = 0;

};

