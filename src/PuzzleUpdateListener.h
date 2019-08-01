#pragma once

#include "PuzzleObject.h"

class PuzzleNode;

class PuzzleUpdateListener
{
public:
	PuzzleUpdateListener();
	virtual ~PuzzleUpdateListener();

	virtual void onPuzzleComplete() = 0;
	virtual void onObjectStateChange(PuzzleObject* Obj) = 0;
	virtual void onNodeActive(PuzzleNode* N) = 0;
	virtual void onNodeComplete(PuzzleNode * N) = 0;
	virtual void onNodeIncomplete(PuzzleNode * N) = 0;
	virtual void onNoEffect(PuzzleObject* Obj) = 0;
};

