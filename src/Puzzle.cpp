#include "Puzzle.h"

namespace PPG
{
	void Puzzle::addNode(Node* n, bool isRelevant)
	{
		nodes.push_back(n);
		if (isRelevant) relevantNodes.push_back(n);

	}

	void Puzzle::setRelation(Relation O)
	{
		relation = O;
	}

	void Puzzle::setNodes(NodeVec nodes)
	{
		nodes = nodes;
		relevantNodes = nodes;
	}

	bool checkCompleted(Node M) {
		return M.isCompleted();
	}

	bool checkActive(Node M) {
		return M.isActive();
	}

	void Puzzle::handleEvent(Event e)
	{
		// TODO: Check if in events

		/* Impleementation wrt Definition 3.8 Formal Description*/
		NodeVec compatibleActiveNodes;
		NodeVec compatibleCompletedNodes;

		// Find compatible nodes
		for (auto& it: nodes) {
			if (isNodeCompatible(it, e)) {
				if (it->isActive()) {
					compatibleActiveNodes.push_back(it);
				}
				else if (it->isCompleted() && relation.checkAllFollowing(it, checkActive)) {
					compatibleCompletedNodes.push_back(it);
				}
			}
		}

		Node* N;
		NodeVec handlingNodes;
		if (!compatibleActiveNodes.empty()) {
			handlingNodes = relation.getSmallestNodesFromList(compatibleActiveNodes);
			N = handlingNodes.at(0);
		}
		else if (!compatibleCompletedNodes.empty()) {
			handlingNodes = relation.getLargestNodesFromList(compatibleCompletedNodes);
			N = handlingNodes.at(0);
		}
		else {
			// No Node found to handle that event.
			if (updateListener != nullptr) {
				updateListener->onNoEffect(e.getRelatedObject());
			}
			return;
		}

		/* Single Time handling, but every "handling" node will update its status and the status of its following nodes*/
		if (N->handleEvent(e) == 0) {// object changed
			if (updateListener != nullptr) {
				updateListener->onObjectStateChange(N->getRelatedObject());
			}
		}
		else {
			if (updateListener != nullptr) {
				updateListener->onNoEffect(N->getRelatedObject());
			}
		}

		for (auto& it2: handlingNodes) {
			it2->updateCompletionState();
			updateNodeProperties(it2);
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

	void Puzzle::setUpdateListener(UpdateListener* PUL)
	{
		this->updateListener = PUL;

		for (auto& n: nodes) {
			n->setPuzzleUpdateListener(updateListener);
		}
	}

	void doSetNodeActive(Node* M) {
		if (M->isIncomplete()) M->setPuzzleNodeState(ENodeState::ACTIVE);
	}

	void doSetNodeIncomplete(Node* M) {
		if (M->isActive()) M->setPuzzleNodeState(ENodeState::INCOMPLETE);
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
		for (auto& it: relevantNodes) {
			if (!it->isCompleted()) {
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

	void Puzzle::addEvent(Event* e)
	{
		this->events.push_back(e);
	}

	NodeVec Puzzle::getNodes() const
	{
		return this->nodes;
	}

	NodeVec Puzzle::getRelevantNodes() const
	{
		return this->relevantNodes;
	}

	EventVec Puzzle::getEvents() const
	{
		return this->events;
	}

	Relation Puzzle::getRelation() const
	{
		return this->relation;
	}

	std::string Puzzle::getSimpleTextualRepresentation() const
	{
		std::string out = "";

		out += "<<< Puzzle >>>\n";
		for (auto& it: nodes) {
			out += it->getSimpleTextualRepresentation();
			out += "\n";
		}

		out += "$$$ Relation $$$ \n";
		out += relation.getSimpleTextualRepresentation();

		out += "<<<<>>>>\n";
		return out;
	}

	std::string Puzzle::getExtendedTextualRepresentation() const
	{
		std::string out = "";

		out += "<<< Puzzle >>>\n";
		for (auto& it: nodes) {
			out += it->getSimpleTextualRepresentation();
			out += "\n";
		}

		out += "$$$ Relation Simple $$$ \n";
		out += relation.getSimpleTextualRepresentation();

		out += "<<<<>>>>\n";

		out += "$$$ Relation Extended $$$ \n";
		out += relation.getExtendedTextualRepresentation(nodes);

		out += "<<<<>>>>\n";

		return out;
	}

	std::string Puzzle::getTextualEnvironmentDescription() const
	{
		std::string out = "";

		out += "<<< Environment >>>\n";
		for (auto& it: nodes) {
			out += it->getRelatedObject()->getObjectName() + " - " + it->getRelatedObject()->getCurrentState()->getStateName() + "\n";
		}
		out += "<<<<>>>>\n";
		return out;
	}

	Vec<GraphNode*> Puzzle::getGraphRepresentation()
	{
		Vec<GraphNode*> allRoots = relation.getGraphRepresentation(nodes);
		return allRoots;
	}

	bool Puzzle::canNodeHandleEvent(Node* N, Event E)
	{
		bool bResult = true;
		bResult = bResult && N->getRelatedObject()->getObjectName() == E.getRelatedObject()->getObjectName();
		bResult = bResult && (N->isActive() || (N->isCompleted() && relation.checkAllFollowing(N, checkActive)));
		// Check object relation
		return bResult;
	}

	bool Puzzle::isNodeCompatible(Node* N, Event E)
	{
		bool bResult = true;
		bResult = bResult && N->getRelatedObject()->getObjectName() == E.getRelatedObject()->getObjectName();
		// Check object relation
		return bResult;
	}
}

