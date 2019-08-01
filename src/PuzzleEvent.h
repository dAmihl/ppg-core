#pragma once

#include "PuzzleObject.h"
#include <string>

class PuzzleEvent
{
public:
	PuzzleEvent(std::string name_v, PuzzleObject* G);
	~PuzzleEvent();
	PuzzleObject* getRelatedObject();
	std::string getEventName();
	bool getIsReversible();
	void setIsReversible(bool newIsReversible);

private:
	PuzzleObject* relatedObject;
	std::string eventName;
	bool isReversible = true;
};

