#pragma once

#include "Object.h"
#include "State.h"
#include "Event.h"
#include "UpdateListener.h"

namespace PPG
{
	enum class PUZZLENODE_STATE {
		INCOMPLETE,
		ACTIVE,
		COMPLETED
	};


	class PPG_EXPORT Node
	{
	public:
		Node(Object* G, State Sg);
		Object* getRelatedObject();
		State getGoalState();
		PUZZLENODE_STATE getCurrentNodeState();
		bool isActive();
		bool isCompleted();
		bool isIncomplete();

		int handleEvent(Event e);
		void updateCompletionState();

		void setPuzzleNodeState(PUZZLENODE_STATE state);

		void setPuzzleUpdateListener(UpdateListener* updList);

		std::string getTextualRepresentation();
		std::string getSimpleTextualRepresentation();

	private:
		Object* relatedObject = NULL;
		State goalState;
		PUZZLENODE_STATE currentNodeState;
		UpdateListener* PUL = NULL;
		std::string getCompletionStateString();

	};

}
