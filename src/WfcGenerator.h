#pragma once

/**
*
* "Kind of"-WFC (Wave Function Collapse) generation.
* The initial state is that every node is potentially connected to every other node. When choosing node pairs at random,
* these are collapsed and potential rules are propagated to the other potential node-pairs
* 
* ==Explanation==
* 
*  - Create matrix for nodes A...X in 2 dimensions
*  - Initialize identities with NOTs (NOTs indicate that these pairs are not allowed)
*  - Initialize NOTs based on Rules (before, after, etc.)
*
*	e.g.
*	_|A|B|C|D
*	A|0|_|_|_|
*	B|_|0|_|_|
*	C|_|_|0|_|
*	D|_|_|_|0|
*
*	0 == NOT
*	* == USED
*	empty = available
*
*	e.g. When choosing new pair (C,D)
*  _|A|B|C|D
*	A|0|_|_|_|
*	B|_|0|_|_|
*	C|_|_|0|0|
*	D|_|_|*|0|
*
*
*  THEN:
*	- Choose available Pair (X,Y) at random
*  - Insert USED into (X,Y) and NOT into (Y,X)
*	- For every W in pair (W,Y), if (W,Y) is NOT, then insert NOT into (W, X)
*		("Every node which was not allowed to reference to Y is now also not allowed to ref to X" - Transitivity)
*		(in short: Copy every NOT in "row" Y to "row" X)
*
*	e.g. When choosing new pair (C,D)
*  _|A|B|C|D
*	A|0|_|_|_|
*	B|_|0|_|_|
*	C|_|_|0|0|
*	D|_|_|*|0|
*
* Then, when choosing (B,C)
*
*  _|A|B|C|D
*	A|0|_|_|_|
*	B|_|0|0|0| also copy NOT from (D,C) into (D,B) because of potential circular dependency of B -> C -> D -!> B
*	C|_|*|0|0|
*	D|_|_|*|0|
*
* Note that USED marks do not count as NOT. e.g. when now choosing (A,D)
*  _|A|B|C|D
*	A|0|_|_|0|
*	B|_|0|_|_|
*	C|_|_|0|0|
*	D|*|_|*|0|
*
* We can still reference (C,A) for example
*	B -> C -> A
*	      \----\-> D
*/


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
#include "core/WfcMat.h"


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

		NodeVec generateNodes(const ObjVec& objects, size_t numNodes);

		void initializeActivePropertyOnNodes(UPtr<Puzzle>& P);
		void cleanupNodes(UPtr<Puzzle>& P);
		void removeNodeFromList(const Ptr<Node>& N, NodeVec& nodes);

	};

}

