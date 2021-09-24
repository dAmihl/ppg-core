#pragma once

#include "Core.h"
#include "Object.h"
#include "State.h"
#include "GraphNode.h"

namespace PPG
{
	class PPG_EXPORT GraphNode
	{
	public:
		Ptr<Object> getObject() const;
		State getState() const;
		Vec<GraphNode*> getChildren() const;

		void setObject(Ptr<Object> object);
		void setState(State state);
		void setChildren(Vec<GraphNode*> children);
		void addChild(GraphNode* child);

	private:
		Ptr<Object> object;
		State state;
		Vec<GraphNode*> children;

	};
}


