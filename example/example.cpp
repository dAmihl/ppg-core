#include <iostream>

#include "PuzzGen.h"

int main()
{
	void* objPtr;
	PPG::Ptr<PPG::Object> oref;
	{
		PPG::Context c;
		auto o1 = c.add<PPG::Object>("Door");
		oref = o1;
		PPG::StateTransition to1;
		PPG::State doorOpen{ "Open" };
		PPG::State doorClosed{ "Closed" };
		to1.addTransition("Use", doorClosed, doorOpen);
		to1.addTransition("Use", doorOpen, doorClosed);
		o1->setStateTransition(to1);

		PPG::Generator generator;
		auto P = generator.generatePuzzle(c);
		objPtr = &*o1;
		std::cout << P->getExtendedTextualRepresentation() << "\n";
	}
	return 0;
}