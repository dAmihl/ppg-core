#pragma once

#include <string>

class PuzzleEventManager
{
public:
	PuzzleEventManager();
	virtual ~PuzzleEventManager();

	virtual void onObjectEvent(std::string objName, std::string eventName) = 0;

};

