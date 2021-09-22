#pragma once

#include "PuzzleObject.h"

namespace PPG
{
	class Node;

	class PuzzleUpdateListener
	{
	public:

		virtual void onPuzzleComplete() = 0;
		virtual void onObjectStateChange(PuzzleObject* Obj) = 0;
		virtual void onNodeActive(Node* N) = 0;
		virtual void onNodeComplete(Node* N) = 0;
		virtual void onNodeIncomplete(Node* N) = 0;
		virtual void onNoEffect(PuzzleObject* Obj) = 0;
	};

}
