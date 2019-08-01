#pragma once

#include <string>

class PuzzleState
{
public:
	PuzzleState();
	PuzzleState(std::string name);
	~PuzzleState();
	std::string getStateName();

private:
	std::string stateName;
};

