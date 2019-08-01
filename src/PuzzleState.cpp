#include "PuzzleState.h"




PuzzleState::PuzzleState()
{
	this->stateName = "DefaultState";
}

PuzzleState::PuzzleState(std::string name)
{
	this->stateName = name;
}

PuzzleState::~PuzzleState()
{
}

std::string PuzzleState::getStateName()
{
	return this->stateName;
}
