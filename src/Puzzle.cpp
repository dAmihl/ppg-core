#include "Puzzle.h"


Puzzle::Puzzle()
{
}


Puzzle::~Puzzle()
{
}

void Puzzle::addNode(PuzzleNode* n, bool isRelevant)
{
	this->nodes.push_back(n);
	if (isRelevant) this->relevantNodes.push_back(n);
	
}

void Puzzle::setRelation(PuzzleRelation O)
{
	this->relation = O;
}

void Puzzle::setNodes(T_PuzzleNodeList nodes)
{
	this->nodes = nodes;
	this->relevantNodes = nodes;
}

bool checkCompleted(PuzzleNode M) {
	return M.isCompleted();
}

bool checkActive(PuzzleNode M) {
	return M.isActive();
}

void Puzzle::handleEvent(PuzzleEvent e)
{
	// TODO: Check if in events

	/* Impleementation wrt Definition 3.8 Formal Description*/
	T_PuzzleNodeList compatibleActiveNodes;
	T_PuzzleNodeList compatibleCompletedNodes;

	// Find compatible nodes
	for (T_PuzzleNodeList::iterator it = this->nodes.begin(); it != this->nodes.end(); ++it) {
		if (isNodeCompatible(*it, e)) {
			if ((*it)->isActive()) {
				compatibleActiveNodes.push_back(*it);
			}
			else if ((*it)->isCompleted() && this->relation.checkAllFollowing((*it), checkActive)) {
				compatibleCompletedNodes.push_back(*it);
			}
		}
	}

	PuzzleNode *N;
	T_PuzzleNodeList handlingNodes;
	if (!compatibleActiveNodes.empty()) {
		handlingNodes = this->relation.getSmallestNodesFromList(compatibleActiveNodes);
		N = handlingNodes.at(0);
	}
	else if (!compatibleCompletedNodes.empty()) {
		handlingNodes = this->relation.getLargestNodesFromList(compatibleCompletedNodes);
		N = handlingNodes.at(0);
	}
	else {
		// No Node found to handle that event.
		if (this->updateListener != nullptr) {
			this->updateListener->onNoEffect(e.getRelatedObject());
		}
		return;
	}
	
	/* Single Time handling, but every "handling" node will update its status and the status of its following nodes*/
	if (N->handleEvent(e) == 0) {// object changed
		if (this->updateListener != nullptr) {
			this->updateListener->onObjectStateChange(N->getRelatedObject());
		}
	}
	else {
		if (this->updateListener != nullptr) {
			this->updateListener->onNoEffect(N->getRelatedObject());
		}
	}

	for (T_PuzzleNodeList::iterator it2 = handlingNodes.begin(); it2 != handlingNodes.end(); ++it2) {
		(*it2)->updateCompletionState();
		updateNodeProperties(*it2);
	}
	
	

	// Find nodes
	/*for (std::vector<PuzzleNode*>::iterator it = this->nodes.begin(); it != this->nodes.end(); ++it) {
		if (canNodeHandleEvent((*it), e)) {
			(*it)->handleEvent(e);
			(*it)->updateCompletionState();
			updateNodeProperties(*it);
		}
	}*/
	checkPuzzleCompletion();
}

void Puzzle::setUpdateListener(PuzzleUpdateListener * PUL)
{
	this->updateListener = PUL;

	for (T_PuzzleNodeList::iterator n = this->nodes.begin(); n != this->nodes.end(); ++n) {
		(*n)->setPuzzleUpdateListener(this->updateListener);
	}

}



void doSetNodeActive(PuzzleNode* M) {
	if (M->isIncomplete()) M->setPuzzleNodeState(PUZZLENODE_STATE::ACTIVE);
}

void doSetNodeIncomplete(PuzzleNode* M) {
	if (M->isActive()) M->setPuzzleNodeState(PUZZLENODE_STATE::INCOMPLETE);
}

void doCheckNodeComplete(PuzzleNode* M) {
	if (!M->isCompleted()) M->updateCompletionState();
}

void Puzzle::updateNodeProperties(PuzzleNode* N)
{
	this->relation.checkDoForAllFollowing(N, checkCompleted,doSetNodeActive);
	//this->relation.checkDoForAllFollowing(N, checkCompleted, doCheckNodeComplete); // check node already complete by definition
	this->relation.checkDoForAllFollowing(N, checkActive, doSetNodeIncomplete);
}

void Puzzle::checkPuzzleCompletion()
{
	for (T_PuzzleNodeList::iterator it = this->relevantNodes.begin(); it != this->relevantNodes.end(); ++it) {
		if (!(*it)->isCompleted()) {
			return;
		}
	}
	onPuzzleCompleted();
}

void Puzzle::onPuzzleCompleted()
{
	if (this->updateListener != nullptr) {
		this->updateListener->onPuzzleComplete();
	}
	
}

void Puzzle::addEvent(PuzzleEvent* e)
{
	this->events.push_back(e);
}

T_PuzzleNodeList Puzzle::getNodes()
{
	return this->nodes;
}

T_PuzzleNodeList Puzzle::getRelevantNodes()
{
	return this->relevantNodes;
}

T_PuzzleEventList Puzzle::getEvents()
{
	return this->events;
}

PuzzleRelation Puzzle::getRelation()
{
	return this->relation;
}

std::string Puzzle::getSimpleTextualRepresentation()
{
	std::string out = "";

	out += "<<< Puzzle >>>\n";
	for (T_PuzzleNodeList::iterator it = this->nodes.begin(); it != this->nodes.end(); ++it) {
		out += (*it)->getSimpleTextualRepresentation();
		out += "\n";
	}

	out += "$$$ Relation $$$ \n";
	out += this->relation.getSimpleTextualRepresentation();

	out += "<<<<>>>>\n";
	return out;
}



std::string Puzzle::getExtendedTextualRepresentation()
{
	std::string out = "";

	out += "<<< Puzzle >>>\n";
	for (T_PuzzleNodeList::iterator it = this->nodes.begin(); it != this->nodes.end(); ++it) {
		out += (*it)->getSimpleTextualRepresentation();
		out += "\n";
	}

	out += "$$$ Relation Simple $$$ \n";
	out += this->relation.getSimpleTextualRepresentation();

	out += "<<<<>>>>\n";

	out += "$$$ Relation Extended $$$ \n";
	out += this->relation.getExtendedTextualRepresentation(this->nodes);

	out += "<<<<>>>>\n";

	return out;
}

std::string Puzzle::getTextualEnvironmentDescription()
{
	std::string out = "";

	out += "<<< Environment >>>\n";
	for (T_PuzzleNodeList::iterator it = this->nodes.begin(); it != this->nodes.end(); ++it) {
		out += (*it)->getRelatedObject()->getObjectName() + " - " + (*it)->getRelatedObject()->getCurrentState().getStateName() + "\n";
	}
	out += "<<<<>>>>\n";
	return out;
}

T_PuzzleGraphNodeList Puzzle::getGraphRepresentation()
{
	T_PuzzleGraphNodeList allRoots = this->relation.getGraphRepresentation(this->nodes);
	return allRoots;
}

bool Puzzle::canNodeHandleEvent(PuzzleNode *N, PuzzleEvent E)
{
	bool bResult = true;
	bResult = bResult && N->getRelatedObject()->getObjectName() == E.getRelatedObject()->getObjectName();
	bResult = bResult && (N->isActive() || (N->isCompleted() && this->relation.checkAllFollowing(N, checkActive)));
	// Check object relation
	return bResult;
}

bool Puzzle::isNodeCompatible(PuzzleNode * N, PuzzleEvent E)
{
	bool bResult = true;
	bResult = bResult && N->getRelatedObject()->getObjectName() == E.getRelatedObject()->getObjectName();
	// Check object relation
	return bResult;
}


