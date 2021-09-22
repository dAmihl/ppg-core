#pragma once

#include "PuzzleObject.h"
#include <string>

class PuzzleEvent
{
public:
	PuzzleEvent(std::string name_v, PuzzleObject* G) 
		: eventName{ name_v }, relatedObject{ G }
		{}

	PuzzleObject* getRelatedObject() const { return relatedObject; };
	std::string getEventName() const { return eventName; };
	bool getIsReversible() const { return isReversible; };
	void setIsReversible(bool newIsReversible) { isReversible = newIsReversible; };

private:
	PuzzleObject* relatedObject;
	std::string eventName;
	bool isReversible = true;
};

