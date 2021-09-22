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

	class Generator
	{
	public:

		Puzzle* generatePuzzle(ObjVec objects, EventVec events, RuleVec rules);

		Relation* simpleGenerateRelation(NodeVec nodes);

		Relation* generateRelation(NodeVec nodes, RuleVec rules);
		Relation* generateRelationExperimental(Puzzle* P, NodeVec nodes, RuleVec rules);

		NodeVec generateNodes(ObjVec objects);

		void setNumberNodes(int n);
		int getNumberNodes();

		void setSeed(unsigned int seed);
		unsigned int getSeed();

	private:

		void initializeActivePropertyOnNodes(Puzzle* P);

		void cleanupNodes(Puzzle* P);
		void removeNodeFromList(Node* N, NodeVec& nodes);
		int NUM_NODES = 0;
		unsigned int seed = 0;
		bool seedSet = false;
	};

}

