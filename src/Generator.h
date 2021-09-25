#pragma once

#include "core/Core.h"
#include "core/Randomizer.h"
#include "core/Node.h"
#include "core/Relation.h"
#include "Puzzle.h"
#include "core/GeneratorHelper.h"
#include "core/Logger.h"
#include "Context.h"
#include <algorithm>

namespace PPG 
{

	using NodeVec = Vec<Ptr<Node>>;
	using ObjVec = Vec<Ptr<Object>>;
	using EventVec = Vec<Ptr<Event>>;
	using RuleVec = Vec<Rule>;

	class PPG_EXPORT Generator
	{
	public:

		Generator() : numberNodes{ 0 }{ }
		Generator(uint32_t numNodes) : numberNodes{ numNodes }{}

		UPtr<Puzzle> generatePuzzle(Context context);

		Relation generateRelation(NodeVec& nodes, RuleVec rules);
		Relation generateRelationExperimental(UPtr<Puzzle>& P, NodeVec& nodes, RuleVec& rules);

		void setSeed(unsigned int seed);
		unsigned int getSeed() const;

	private:

		NodeVec generateNodes(const ObjVec& objects, size_t numNodes);

		void initializeActivePropertyOnNodes(UPtr<Puzzle>& P);

		void cleanupNodes(UPtr<Puzzle>& P);
		void removeNodeFromList(const Ptr<Node>& N, NodeVec& nodes);
		unsigned int seed = 0;
		uint32_t numberNodes;
		bool seedSet = false;
	};

}

