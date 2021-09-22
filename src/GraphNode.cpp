#include "GraphNode.h"

namespace PPG
{
	Object* GraphNode::getObject() const
	{
		return this->object;
	}

	State* GraphNode::getState() const
	{
		return this->state;
	}

	Vec<GraphNode*> GraphNode::getChildren() const
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



