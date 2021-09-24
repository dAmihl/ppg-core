#include "Node.h"

namespace PPG
{

	Node::Node(Object* G, State Sg) : relatedObject{ G }, goalState{ Sg }, currentNodeState{ENodeState::INCOMPLETE}
	{}

	PPG::Object* Node::getRelatedObject() const
{
		return relatedObject;
	}

	const PPG::State Node::getGoalState() const
{
		return goalState;
	}

	ENodeState Node::getCurrentNodeState() const
	{
		return currentNodeState;
	}

	bool Node::isActive() const
	{
		return currentNodeState == ENodeState::ACTIVE;
	}

	bool Node::isCompleted() const
	{
		return currentNodeState == ENodeState::COMPLETED;
	}

	bool Node::isIncomplete() const
	{
		return currentNodeState == ENodeState::INCOMPLETE;
	}


	int Node::handleEvent(Event e)
	{
		State Sc = relatedObject->getCurrentState();
		EventMapVal vec = relatedObject->getStateTransition().findTransitions(e.getEventName());
		for (auto& vecIt : vec) {
			if (vecIt.first == Sc) { // check name equality
				State nextState = vecIt.second;
				relatedObject->setCurrentState(nextState); // next state set
				return 0;
				break;
			}
			// Test : Two Way state transitions, i.e. reversible state transitions
			if (e.getIsReversible()) {
				if (vecIt.second == Sc) { // check name equality
					State nextState = vecIt.first;
					relatedObject->setCurrentState(nextState); // next state set
					return 0;
					break;
				}
			}
			//return 3;
		}
		return 1;
	}

	Str Node::getCompletionStateString() const {
		Str out;
		switch (currentNodeState) {
		case ENodeState::ACTIVE: out = "ACTIVE"; break;
		case ENodeState::INCOMPLETE: out = "INCOMPLETE"; break;
		case ENodeState::COMPLETED: out = "COMPLETED"; break;
		default: out = "UNKNOWN"; break;
		}
		return out;
	}

	Str Node::getTextualRepresentation() const
	{
		Str out = "";
		out += "<> Puzzlenode (" + relatedObject->getObjectName() + " | " + goalState.getName() + ") <> \n";
		out += relatedObject->getTextualRepresentation();
		out += "Puzzlenode-State: ";
		out += getCompletionStateString();
		out += "\n";
		return out;
	}
	Str Node::getSimpleTextualRepresentation() const
	{
		Str out = ">> Puzzlenode (" + relatedObject->getObjectName() + " | " + goalState.getName() + ") :: " + getCompletionStateString() + " <> \n";
		return out;
	}

	// Only callable privately after handling event!
	void Node::updateCompletionState()
	{
		if (relatedObject->getCurrentState() == goalState) { // name equality --> TODO
			setPuzzleNodeState(ENodeState::COMPLETED);
		}

		// Traverse back in puzzle "graph"
		else if (relatedObject->getCurrentState() != goalState) {
			if (currentNodeState == ENodeState::COMPLETED) {
				setPuzzleNodeState(ENodeState::ACTIVE);
			}
		}
	}

	void Node::setPuzzleNodeState(ENodeState state)
	{
		ENodeState oldState = currentNodeState;
		if (state == oldState) return;

		currentNodeState = state;

		if (PUL == nullptr) return;

		if (state == ENodeState::ACTIVE) {
			PUL->onNodeActive(this);
		}
		if (state == ENodeState::INCOMPLETE) {
			PUL->onNodeIncomplete(this);
		}
		if (state == ENodeState::COMPLETED) {
			PUL->onNodeComplete(this);
		}
	}

	void Node::setPuzzleUpdateListener(UpdateListener* updList)
	{
		PUL = updList;
	}
}
