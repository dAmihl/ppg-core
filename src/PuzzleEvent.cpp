#include "PuzzleEvent.h"




PuzzleEvent::PuzzleEvent(std::string name_v, PuzzleObject* G)
{
	this->relatedObject = G;
	this->eventName = name_v;
}

PuzzleEvent::~PuzzleEvent()
{
}

PuzzleObject* PuzzleEvent::getRelatedObject()
{
	return this->relatedObject;
}

std::string PuzzleEvent::getEventName()
{
	return this->eventName;
}

bool PuzzleEvent::getIsReversible()
{
	return this->isReversible;
}

void PuzzleEvent::setIsReversible(bool newIsReversible)
{
	this->isReversible = newIsReversible;
}
