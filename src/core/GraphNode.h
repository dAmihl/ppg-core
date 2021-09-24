#pragma once

#include "Core.h"
#include "../Object.h"
#include "../State.h"
#include "GraphNode.h"

namespace PPG
{
	class PPG_EXPORT GraphNode
	{
	public:

		GraphNode(Object& obj, State s) 
			: object{ obj }, state{ s }
		{}

		Object& getObject() const;
		State getState() const;
		Vec<GraphNode*> getChildren() const;

		void setChildren(Vec<GraphNode*> children);
		void addChild(GraphNode* child);

	private:
		Object& object;
		State state;
		Vec<GraphNode*> children;

	};
}


