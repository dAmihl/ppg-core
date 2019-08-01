#include "PuzzleNode.h"


PuzzleNode::~PuzzleNode()
{
}


PuzzleNode::PuzzleNode(PuzzleObject* G, PuzzleState Sg)
{
	this->relatedObject = G;
	this->goalState = Sg;
	this->currentNodeState = PUZZLENODE_STATE::INCOMPLETE;
}


PuzzleObject* PuzzleNode::getRelatedObject()
{
	return this->relatedObject;
}

PuzzleState PuzzleNode::getGoalState()
{
	return this->goalState;
}

PUZZLENODE_STATE PuzzleNode::getCurrentNodeState()
{
	return this->currentNodeState;
}

bool PuzzleNode::isActive()
{
	return this->currentNodeState == PUZZLENODE_STATE::ACTIVE;
}

bool PuzzleNode::isCompleted()
{
	return this->currentNodeState == PUZZLENODE_STATE::COMPLETED;
}

bool PuzzleNode::isIncomplete()
{
	return this->currentNodeState == PUZZLENODE_STATE::INCOMPLETE;
}


int PuzzleNode::handleEvent(PuzzleEvent e)
{	
	PuzzleState Sc = this->relatedObject->getCurrentState();
	std::vector<std::pair<PuzzleState, PuzzleState>> vec = this->relatedObject->getStateTransition().findTransitions(e.getEventName());
	for (std::vector<std::pair<PuzzleState, PuzzleState>>::iterator vecIt = vec.begin(); vecIt != vec.end(); ++vecIt) {
		if (vecIt->first.getStateName() == Sc.getStateName()) { // check name equality
			PuzzleState nextState = vecIt->second;
			this->relatedObject->setCurrentState(nextState); // next state set
			return 0;
			break;
		}
		// Test : Two Way state transitions, i.e. reversible state transitions
		if (e.getIsReversible()) {
			if (vecIt->second.getStateName() == Sc.getStateName()) { // check name equality
				PuzzleState nextState = vecIt->first;
				this->relatedObject->setCurrentState(nextState); // next state set
				return 0;
				break;
			}
		}
		//return 3;
	}
	return 1;
}




std::string PuzzleNode::getCompletionStateString() {
	std::string out;
	switch (this->currentNodeState) {
	case PUZZLENODE_STATE::ACTIVE: out = "ACTIVE"; break;
	case PUZZLENODE_STATE::INCOMPLETE: out = "INCOMPLETE"; break;
	case PUZZLENODE_STATE::COMPLETED: out = "COMPLETED"; break;
	default: out = "UNKNOWN"; break;
	}
	return out;
}

std::string PuzzleNode::getTextualRepresentation()
{
	std::string out = "";
	out += "<> Puzzlenode (" + this->relatedObject->getObjectName() + " | " + this->goalState.getStateName() + ") <> \n";
	out += this->relatedObject->getTextualRepresentation();
	out += "Puzzlenode-State: ";
	out += getCompletionStateString();
	out += "\n";
	return out;
}
std::string PuzzleNode::getSimpleTextualRepresentation()
{
	std::string out = ">> Puzzlenode (" + this->relatedObject->getObjectName() + " | " + this->goalState.getStateName() + ") :: "+getCompletionStateString()+" <> \n";
	return out;
}
// Only callable privately after handling event!
void PuzzleNode::updateCompletionState()
{
	if (relatedObject->getCurrentState().getStateName() == this->goalState.getStateName()) { // name equality --> TODO
		this->setPuzzleNodeState(PUZZLENODE_STATE::COMPLETED);
	}
	// Traverse back in puzzle "graph"
	else if (relatedObject->getCurrentState().getStateName() != this->goalState.getStateName()) {
		if (this->currentNodeState == PUZZLENODE_STATE::COMPLETED) {
			this->setPuzzleNodeState(PUZZLENODE_STATE::ACTIVE);
		}
	}
}

void PuzzleNode::setPuzzleNodeState(PUZZLENODE_STATE state)
{
	PUZZLENODE_STATE oldState = this->currentNodeState;
	if (state == oldState) return;

	this->currentNodeState = state;
	
	if (this->PUL == nullptr) return;

	if (state == PUZZLENODE_STATE::ACTIVE) {
		this->PUL->onNodeActive(this);
	}
	if (state == PUZZLENODE_STATE::INCOMPLETE) {
		this->PUL->onNodeIncomplete(this);
	}
	if (state == PUZZLENODE_STATE::COMPLETED) {
		this->PUL->onNodeComplete(this);
	}
}

void PuzzleNode::setPuzzleUpdateListener(PuzzleUpdateListener * updList)
{
	this->PUL = updList;
}



