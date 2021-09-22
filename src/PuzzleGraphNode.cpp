#include "PuzzleGraphNode.h"

namespace PPG
{
	PuzzleObject* PuzzleGraphNode::getObject()
	{
		return this->object;
	}

	PuzzleState* PuzzleGraphNode::getState()
	{
		return this->state;
	}

	Vec<PuzzleGraphNode*> PuzzleGraphNode::getChildren()
	{
		return this->children;
	}

	void PuzzleGraphNode::setObject(PuzzleObject* object)
	{
		this->object = object;
	}

	void PuzzleGraphNode::setState(PuzzleState* state)
	{
		this->state = state;
	}

	void PuzzleGraphNode::setChildren(Vec<PuzzleGraphNode*> children)
	{
		this->children = children;
	}

	void PuzzleGraphNode::addChild(PuzzleGraphNode* child)
	{
		children.push_back(child);
	}
}



