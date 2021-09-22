#include "Puzzle.h"

namespace PPG
{
	void Puzzle::addNode(Node* n, bool isRelevant)
	{
		this->nodes.push_back(n);
		if (isRelevant) this->relevantNodes.push_back(n);

	}

	void Puzzle::setRelation(PuzzleRelation O)
	{
		this->relation = O;
	}

	void Puzzle::setNodes(NodeVec nodes)
	{
		this->nodes = nodes;
		this->relevantNodes = nodes;
	}

	bool checkCompleted(Node M) {
		return M.isCompleted();
	}

	bool checkActive(Node M) {
		return M.isActive();
	}

	void Puzzle::handleEvent(PuzzleEvent e)
	{
		// TODO: Check if in events

		/* Impleementation wrt Definition 3.8 Formal Description*/
		NodeVec compatibleActiveNodes;
		NodeVec compatibleCompletedNodes;

		// Find compatible nodes
		for (NodeVec::iterator it = this->nodes.begin(); it != this->nodes.end(); ++it) {
			if (isNodeCompatible(*it, e)) {
				if ((*it)->isActive()) {
					compatibleActiveNodes.push_back(*it);
				}
				else if ((*it)->isCompleted() && this->relation.checkAllFollowing((*it), checkActive)) {
					compatibleCompletedNodes.push_back(*it);
				}
			}
		}

		Node* N;
		NodeVec handlingNodes;
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

		for (NodeVec::iterator it2 = handlingNodes.begin(); it2 != handlingNodes.end(); ++it2) {
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

	void Puzzle::setUpdateListener(PuzzleUpdateListener* PUL)
	{
		this->updateListener = PUL;

		for (NodeVec::iterator n = this->nodes.begin(); n != this->nodes.end(); ++n) {
			(*n)->setPuzzleUpdateListener(this->updateListener);
		}

	}



	void doSetNodeActive(Node* M) {
		if (M->isIncomplete()) M->setPuzzleNodeState(PUZZLENODE_STATE::ACTIVE);
	}

	void doSetNodeIncomplete(Node* M) {
		if (M->isActive()) M->setPuzzleNodeState(PUZZLENODE_STATE::INCOMPLETE);
	}

	void doCheckNodeComplete(Node* M) {
		if (!M->isCompleted()) M->updateCompletionState();
	}

	void Puzzle::updateNodeProperties(Node* N)
	{
		this->relation.checkDoForAllFollowing(N, checkCompleted, doSetNodeActive);
		//this->relation.checkDoForAllFollowing(N, checkCompleted, doCheckNodeComplete); // check node already complete by definition
		this->relation.checkDoForAllFollowing(N, checkActive, doSetNodeIncomplete);
	}

	void Puzzle::checkPuzzleCompletion()
	{
		for (NodeVec::iterator it = this->relevantNodes.begin(); it != this->relevantNodes.end(); ++it) {
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

	NodeVec Puzzle::getNodes()
	{
		return this->nodes;
	}

	NodeVec Puzzle::getRelevantNodes()
	{
		return this->relevantNodes;
	}

	EventVec Puzzle::getEvents()
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
		for (NodeVec::iterator it = this->nodes.begin(); it != this->nodes.end(); ++it) {
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
		for (NodeVec::iterator it = this->nodes.begin(); it != this->nodes.end(); ++it) {
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
		for (NodeVec::iterator it = this->nodes.begin(); it != this->nodes.end(); ++it) {
			out += (*it)->getRelatedObject()->getObjectName() + " - " + (*it)->getRelatedObject()->getCurrentState().getStateName() + "\n";
		}
		out += "<<<<>>>>\n";
		return out;
	}

	Vec<PuzzleGraphNode*> Puzzle::getGraphRepresentation()
	{
		Vec<PuzzleGraphNode*> allRoots = this->relation.getGraphRepresentation(this->nodes);
		return allRoots;
	}

	bool Puzzle::canNodeHandleEvent(Node* N, PuzzleEvent E)
	{
		bool bResult = true;
		bResult = bResult && N->getRelatedObject()->getObjectName() == E.getRelatedObject()->getObjectName();
		bResult = bResult && (N->isActive() || (N->isCompleted() && this->relation.checkAllFollowing(N, checkActive)));
		// Check object relation
		return bResult;
	}

	bool Puzzle::isNodeCompatible(Node* N, PuzzleEvent E)
	{
		bool bResult = true;
		bResult = bResult && N->getRelatedObject()->getObjectName() == E.getRelatedObject()->getObjectName();
		// Check object relation
		return bResult;
	}
}

