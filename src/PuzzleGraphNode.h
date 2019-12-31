#pragma once

#include "PUZZGEN_TYPES.h"
#include "PuzzleState.h"
#include "PuzzleObject.h"
#include "PuzzleEvent.h"

class PuzzleGraphNode
{
public:
	PuzzleGraphNode();
	~PuzzleGraphNode();

	PuzzleObject getObject();
	PuzzleState getState();
	T_PuzzleGraphNodeList getChildren();

	void setObject(PuzzleObject object);
	void setState(PuzzleState state);
	void setChildren(T_PuzzleGraphNodeList children);
	void addChild(PuzzleGraphNode* child);

private:
	PuzzleObject object;
	PuzzleState state;
	T_PuzzleGraphNodeList children;

};

