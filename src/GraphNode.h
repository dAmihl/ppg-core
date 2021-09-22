#pragma once

#include "PuzzGenCore.h"
#include "Object.h"
#include "State.h"
#include "GraphNode.h"

namespace PPG
{
	class PPG_EXPORT GraphNode
	{
	public:
		Object* getObject();
		State* getState();
		Vec<GraphNode*> getChildren();

		void setObject(Object* object);
		void setState(State* state);
		void setChildren(Vec<GraphNode*> children);
		void addChild(GraphNode* child);

	private:
		Object* object;
		State* state;
		Vec<GraphNode*> children;

	};
}


