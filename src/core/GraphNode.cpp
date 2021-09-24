#include "GraphNode.h"

namespace PPG
{
	Object& GraphNode::getObject() const
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

	void GraphNode::setChildren(Vec<GraphNode*> v)
	{
		children = v;
	}

	void GraphNode::addChild(GraphNode* child)
	{
		children.push_back(child);
	}
}



