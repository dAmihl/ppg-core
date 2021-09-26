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
	class WfcGenerator : public Generator
	{
	public:
		WfcGenerator() = default;

		WfcGenerator(unsigned int numNodes) : Generator(numNodes)
		{}

		UPtr<Puzzle> generatePuzzle(Context context) override;
		Relation generateRelation(UPtr<Puzzle>& P, NodeVec& nodes, RuleVec& rules) override;

		NodeVec generateUniqueNodes(const ObjVec& objects, size_t numNodes);

	private:
		void initializeActivePropertyOnNodes(UPtr<Puzzle>& P);
		void cleanupNodes(UPtr<Puzzle>& P);
		void removeNodeFromList(const Ptr<Node>& N, NodeVec& nodes);

	};

}

