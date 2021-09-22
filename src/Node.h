#pragma once

#include "PuzzleObject.h"
#include "PuzzleState.h"
#include "PuzzleEvent.h"
#include "PuzzleUpdateListener.h"

namespace PPG
{
	enum class PUZZLENODE_STATE {
		INCOMPLETE,
		ACTIVE,
		COMPLETED
	};


	class Node
	{
	public:
		Node(PuzzleObject* G, PuzzleState Sg);
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

}
