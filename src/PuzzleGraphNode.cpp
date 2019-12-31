#include "PuzzleGraphNode.h"


PuzzleGraphNode::PuzzleGraphNode()
{
}


PuzzleGraphNode::~PuzzleGraphNode()
{
}

PuzzleObject PuzzleGraphNode::getObject()
{
	return this->object;
}

PuzzleState PuzzleGraphNode::getState()
{
	return this->state;
}

T_PuzzleGraphNodeList PuzzleGraphNode::getChildren()
{
	return this->children;
}

void PuzzleGraphNode::setObject(PuzzleObject object)
{
	this->object = object;
}

void PuzzleGraphNode::setState(PuzzleState state)
{
	this->state = state;
}

void PuzzleGraphNode::setChildren(T_PuzzleGraphNodeList children)
{
	this->children = children;
}

void PuzzleGraphNode::addChild(PuzzleGraphNode* child)
{
	this->children.push_back(child);
}

