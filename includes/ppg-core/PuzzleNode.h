#pragma once

#include "PuzzleObject.h"
#include "PuzzleState.h"
#include "PuzzleEvent.h"
#include "PuzzleUpdateListener.h"

enum PUZZLENODE_STATE {
	INCOMPLETE,
	ACTIVE,
	COMPLETED
};


class PuzzleNode
{
public:
	PuzzleNode(PuzzleObject* G, PuzzleState Sg);
	~PuzzleNode();
	PuzzleObject* getRelatedObject();
	PuzzleState getGoalState();
	PUZZLENODE_STATE getCurrentNodeState();
	bool isActive();
	bool isCompleted();
	bool isIncomplete();

	int handleEvent(PuzzleEvent e);
	void updateCompletionState();

	void setPuzzleNodeState(PUZZLENODE_STATE state);

	void setPuzzleUpdateListener(PuzzleUpdateListener* updList);

	std::string getTextualRepresentation();
	std::string getSimpleTextualRepresentation();

private:
	PuzzleObject* relatedObject = NULL;
	PuzzleState goalState;
	PUZZLENODE_STATE currentNodeState;
	PuzzleUpdateListener* PUL = NULL;
	std::string getCompletionStateString();

};

