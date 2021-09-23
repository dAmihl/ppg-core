#pragma once
#include <vector>
#include "Node.h"
#include "Relation.h"
#include "Randomizer.h"
#include "PuzzGenCore.h"
#include "Puzzle.h"
#include "GeneratorHelper.h"
#include "Logger.h"

namespace PPG 
{

	using NodeVec = Vec<Node*>;
	using ObjVec = Vec<Object*>;
	using EventVec = Vec<Event*>;
	using RuleVec = Vec<Rule>;

	class PPG_EXPORT Generator
	{
	public:

		Generator() : numberNodes{ 0 }{ }
		Generator(uint32_t numNodes) : numberNodes{ numNodes }{}

		Puzzle* generatePuzzle(ObjVec objects, EventVec events, RuleVec rules);

		Relation* simpleGenerateRelation(NodeVec nodes);

		Relation* generateRelation(NodeVec nodes, RuleVec rules);
		Relation* generateRelationExperimental(Puzzle* P, NodeVec nodes, RuleVec rules);

		NodeVec generateNodes(ObjVec objects, size_t numNodes);

		void setSeed(unsigned int seed);
		unsigned int getSeed() const;

	private:

		void initializeActivePropertyOnNodes(Puzzle* P);

		void cleanupNodes(Puzzle* P);
		void removeNodeFromList(Node* N, NodeVec& nodes);
		unsigned int seed = 0;
		uint32_t numberNodes;
		bool seedSet = false;
	};

}

