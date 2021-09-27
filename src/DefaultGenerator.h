#pragma once

#include "core/Core.h"
#include "core/Randomizer.h"
#include "core/Node.h"
#include "core/Relation.h"
#include "Puzzle.h"
#include "core/Generator.h"
#include "core/GeneratorHelper.h"
#include "core/Logger.h"
#include "Context.h"
#include <algorithm>
#include <functional>

namespace PPG 
{

	class PPG_EXPORT DefaultGenerator : public Generator
	{
	public:
		DefaultGenerator() = default;

		DefaultGenerator(unsigned int numNodes) : Generator(numNodes)
		{}

		UPtr<Puzzle> generatePuzzle(Context context) override;
		PPG::Relation DefaultGenerator::generateRelationSimple(NodeVec& nodes, RuleVec rules);
		Relation generateRelation(UPtr<Puzzle>& P, NodeVec& nodes, RuleVec& rules) override;

	private:

		NodeVec generateNodes(const ObjVec& objects, size_t numNodes);

		void initializeActivePropertyOnNodes(UPtr<Puzzle>& P);
		void cleanupNodes(UPtr<Puzzle>& P);
		void removeNodeFromList(const Ptr<Node>& N, NodeVec& nodes);

		
	};

}

