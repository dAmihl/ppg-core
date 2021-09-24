#include "Puzzle.h"

namespace PPG
{
	void Puzzle::addNode(Ptr<Node> n)
	{
		nodes.push_back(n);
	}

	void Puzzle::setRelation(Relation O)
	{
		relation = O;
	}

	void Puzzle::setNodes(NodeVec ns)
	{
		nodes = ns;
	}

	bool checkCompleted(const Node& M) {
		return M.isCompleted();
	}

	bool checkActive(const Node& M) {
		return M.isActive();
	}

	void Puzzle::handleEvent(Event e)
	{
		// TODO: Check if in events

		/* Implementation wrt Definition 3.8 Formal Description*/
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

		Ptr<Node> N;
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

	void doSetNodeActive(Node& M) {
		if (M.isIncomplete()) M.setPuzzleNodeState(ENodeState::ACTIVE);
	}

	void doSetNodeIncomplete(Node& M) {
		if (M.isActive()) M.setPuzzleNodeState(ENodeState::INCOMPLETE);
	}

	void doCheckNodeComplete(Node& M) {
		if (!M.isCompleted()) M.updateCompletionState();
	}

	void Puzzle::updateNodeProperties(Ptr<Node>& N)
	{
		this->relation.checkDoForAllFollowing(N, checkCompleted, doSetNodeActive);
		//this->relation.checkDoForAllFollowing(N, checkCompleted, doCheckNodeComplete); // check node already complete by definition
		this->relation.checkDoForAllFollowing(N, checkActive, doSetNodeIncomplete);
	}

	void Puzzle::checkPuzzleCompletion() const
	{
		for (auto& it: nodes) {
			if (!it->isCompleted()) {
				return;
			}
		}
		onPuzzleCompleted();
	}

	void Puzzle::onPuzzleCompleted() const
	{
		if (this->updateListener != nullptr) {
			this->updateListener->onPuzzleComplete();
		}
	}

	NodeVec& Puzzle::getNodes()
{
		return this->nodes;
	}

	PPG::Relation& Puzzle::getRelation()
{
		return relation;
	}

	Str Puzzle::getSimpleTextualRepresentation() const
	{
		Str out = "";

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

	Str Puzzle::getExtendedTextualRepresentation() const
	{
		Str out = "";

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

	Str Puzzle::getTextualEnvironmentDescription() const
	{
		Str out = "";

		out += "<<< Environment >>>\n";
		for (auto& it: nodes) {
			out += it->getRelatedObject().getObjectName() + " - " + it->getRelatedObject().getCurrentState().getName() + "\n";
		}
		out += "<<<<>>>>\n";
		return out;
	}

	PPG::Vec<GraphNode*> Puzzle::getGraphRepresentation() const
{
		Vec<GraphNode*> allRoots = relation.getGraphRepresentation(nodes);
		return allRoots;
	}

	bool Puzzle::canNodeHandleEvent(const Ptr<Node>& N, Event E) const
	{
		bool bResult = true;
		bResult = bResult && N->getRelatedObject().getObjectName() == E.getRelatedObject().getObjectName();
		bResult = bResult && (N->isActive() || (N->isCompleted() && relation.checkAllFollowing(N, checkActive)));
		// Check object relation
		return bResult;
	}

	bool Puzzle::isNodeCompatible(const Ptr<Node>& N, Event E) const
	{
		bool bResult = true;
		bResult = bResult && N->getRelatedObject().getObjectName() == E.getRelatedObject().getObjectName();
		// Check object relation
		return bResult;
	}
}

