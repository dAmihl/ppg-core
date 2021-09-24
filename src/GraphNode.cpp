#include "GraphNode.h"

namespace PPG
{
	Ptr<Object> GraphNode::getObject() const
	{
		return object;
	}

	State GraphNode::getState() const
	{
		return state;
	}

	Vec<GraphNode*> GraphNode::getChildren() const
	{
		return children;
	}

	void GraphNode::setObject(Ptr<Object> obj)
	{
		object = obj;
	}

	void GraphNode::setState(State s)
	{
		state = s;
	}

	void GraphNode::setChildren(Vec<GraphNode*> v)
	{
		children = v;
	}

	void GraphNode::addChild(GraphNode* child)
	{
		children.push_back(child);
	}
}



