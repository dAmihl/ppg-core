#pragma once

#include "Object.h"

namespace PPG
{
	class Node;

	class UpdateListener
	{
	public:

		virtual void onPuzzleComplete() = 0;
		virtual void onObjectStateChange(Ptr<Object> Obj) = 0;
		virtual void onNodeActive(Node* N) = 0;
		virtual void onNodeComplete(Node* N) = 0;
		virtual void onNodeIncomplete(Node* N) = 0;
		virtual void onNoEffect(Ptr<Object> Obj) = 0;
	};

}
