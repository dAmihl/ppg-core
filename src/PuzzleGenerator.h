#pragma once
#include <vector>
#include "Node.h"
#include "PuzzleRelation.h"
#include "PuzzleRandomizer.h"
#include "PuzzGenCore.h"
#include "Puzzle.h"
#include "PuzzleGeneratorHelper.h"
#include "PuzzleLogger.h"

namespace PPG 
{

	using NodeVec = Vec<Node*>;
	using ObjVec = Vec<PuzzleObject*>;
	using EventVec = Vec<PuzzleEvent*>;
	using RuleVec = Vec<PuzzleRule>;

	class PuzzleGenerator
	{
	public:

		Puzzle* generatePuzzle(ObjVec objects, EventVec events, RuleVec rules);

		PuzzleRelation* simpleGenerateRelation(NodeVec nodes);

		PuzzleRelation* generateRelation(NodeVec nodes, RuleVec rules);
		PuzzleRelation* generateRelationExperimental(Puzzle* P, NodeVec nodes, RuleVec rules);

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

