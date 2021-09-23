#pragma once

#include "Object.h"
#include "State.h"
#include "Event.h"
#include "UpdateListener.h"

namespace PPG
{
	enum class ENodeState {
		INCOMPLETE,
		ACTIVE,
		COMPLETED
	};

	class PPG_EXPORT Node
	{
	public:
		Node(Object* G, State* Sg);
		Object* getRelatedObject() const;
		const State* getGoalState() const;
		ENodeState getCurrentNodeState() const;
		bool isActive() const;
		bool isCompleted() const;
		bool isIncomplete() const;

		int handleEvent(Event e);
		void updateCompletionState();
		void setPuzzleNodeState(ENodeState state);
		void setPuzzleUpdateListener(UpdateListener* updList);

		std::string getTextualRepresentation() const;
		std::string getSimpleTextualRepresentation() const;

	private:
		Object* relatedObject = nullptr;
		State* goalState;
		ENodeState currentNodeState;
		UpdateListener* PUL = nullptr;
		std::string getCompletionStateString() const;

	};


}