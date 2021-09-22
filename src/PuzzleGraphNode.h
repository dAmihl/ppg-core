#pragma once

#include "PuzzGenCore.h"

namespace PPG
{
	class PuzzleGraphNode
	{
	public:
		PuzzleObject* getObject();
		PuzzleState* getState();
		Vec<PuzzleGraphNode*> getChildren();

		void setObject(PuzzleObject* object);
		void setState(PuzzleState* state);
		void setChildren(Vec<PuzzleGraphNode*> children);
		void addChild(PuzzleGraphNode* child);

	private:
		PuzzleObject* object;
		PuzzleState* state;
		Vec<PuzzleGraphNode*> children;

	};
}


