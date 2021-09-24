#pragma once

#include "Core.h"
#include "Randomizer.h"
#include "Node.h"
#include "Relation.h"
#include "Puzzle.h"
#include "GeneratorHelper.h"
#include "Logger.h"
#include "Context.h"

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

		Puzzle* generatePuzzle(ObjVec& objects, EventVec& events, RuleVec& rules);

		Puzzle* generatePuzzle(Context& context);

		Relation generateRelation(NodeVec nodes, RuleVec rules);
		Relation generateRelationExperimental(Puzzle* P, NodeVec nodes, RuleVec& rules);

		void setSeed(unsigned int seed);
		unsigned int getSeed() const;

	private:

		NodeVec generateNodes(const ObjVec& objects, size_t numNodes);

		void initializeActivePropertyOnNodes(Puzzle* P);

		void cleanupNodes(Puzzle* P);
		void removeNodeFromList(Ptr<Node> N, NodeVec& nodes);
		unsigned int seed = 0;
		uint32_t numberNodes;
		bool seedSet = false;
	};

}

