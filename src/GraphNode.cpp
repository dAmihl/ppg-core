#include "GraphNode.h"

namespace PPG
{
	Object* GraphNode::getObject()
	{
		return this->object;
	}

	State* GraphNode::getState()
	{
		return this->state;
	}

	Vec<GraphNode*> GraphNode::getChildren()
	{
		return this->children;
	}

	void GraphNode::setObject(Object* object)
	{
		this->object = object;
	}

	void GraphNode::setState(State* state)
	{
		this->state = state;
	}

	void GraphNode::setChildren(Vec<GraphNode*> children)
	{
		this->children = children;
	}

	void GraphNode::addChild(GraphNode* child)
	{
		children.push_back(child);
	}
}



