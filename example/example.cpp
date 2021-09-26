#include <iostream>

#include "PuzzGen.h"


PPG::Context createSimpleContext()
{
	PPG::Context c;
	auto o1 = c.add<PPG::Object>("Door");
	PPG::StateTransition to1;
	PPG::State doorOpen{ "Open" };
	PPG::State doorClosed{ "Closed" };
	to1.addTransition("Use", doorClosed, doorOpen);
	to1.addTransition("Use", doorOpen, doorClosed);
	o1->setStateTransition(to1);

	return c;
}


PPG::Context createExtendedContext()
{
	PPG::Context con;

	PPG::StateTransition to1;
	PPG::State open{ "Open" };
	PPG::State closed{ "Closed" };
	to1.addTransition("Use", closed, open);
	to1.addTransition("Use", open, closed);
	

	auto a = con.add<PPG::Object>("A");
	a->setCurrentState(closed);
	a->setStateTransition(to1);
	auto b = con.add<PPG::Object>("B");
	a->setCurrentState(closed);
	b->setStateTransition(to1);
	auto c = con.add<PPG::Object>("C");
	c->setCurrentState(closed);
	c->setStateTransition(to1);
	auto d = con.add<PPG::Object>("D");
	d->setCurrentState(closed);
	d->setStateTransition(to1);
	auto e = con.add<PPG::Object>("E");
	e->setCurrentState(closed);
	e->setStateTransition(to1);
	auto f = con.add<PPG::Object>("F");
	f->setCurrentState(closed);
	f->setStateTransition(to1);

	// a < b
	auto ab = PPG::Rule(a, PPG::STATE_ANY, b, PPG::STATE_ANY, PPG::Rule::EPuzzleRuleType::BEFORE);
	// e < f
	auto ef = PPG::Rule(e, PPG::STATE_ANY, f, PPG::STATE_ANY, PPG::Rule::EPuzzleRuleType::BEFORE);


	con.addRule(ab);
	con.addRule(ef);

	return con;
}

int main()
{
	
	// Manual example
	{
		PPG::Context c = createSimpleContext();
		PPG::DefaultGenerator generator;
		auto P = generator.generatePuzzle(c);
		std::cout << P->getExtendedTextualRepresentation() << "\n";
	}


	// WFC
	{
		
		PPG::Context c = createExtendedContext();
		PPG::WfcGenerator generator;
		auto P = generator.generatePuzzle(c);
		std::cout << P->getExtendedTextualRepresentation() << "\n";
	}

	return 0;
}