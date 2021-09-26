#pragma once

#include "Core.h"
#include "Randomizer.h"
#include "Node.h"
#include "Relation.h"
#include "../Puzzle.h"

namespace PPG
{

	using NodeVec = Vec<Ptr<Node>>;
	using ObjVec = Vec<Ptr<Object>>;
	using EventVec = Vec<Ptr<Event>>;
	using RuleVec = Vec<Rule>;

	class Generator
	{
	public:

		Generator() : numberNodes{ 0 }
		{}

		Generator(uint32_t numNodes) : numberNodes{ numNodes }
		{}

		virtual UPtr<Puzzle> generatePuzzle(Context context) = 0;
		virtual Relation generateRelation(UPtr<Puzzle>& P, NodeVec& nodes, RuleVec& rules) = 0;


		void setSeed(unsigned int s) { seed = s; };
		unsigned int getSeed() const { return seed; };

	protected:
		unsigned int seed = 0;
		uint32_t numberNodes;
		bool seedSet = false;
	};

}

