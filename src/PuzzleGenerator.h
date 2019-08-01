#pragma once
#include <vector>
#include "PuzzleNode.h"
#include "PuzzleRelation.h"
#include "PuzzleRandomizer.h"
#include "PUZZGEN_TYPES.h"
#include "Puzzle.h"

class PuzzleGenerator
{
public:
	PuzzleGenerator();
	~PuzzleGenerator();
	
	Puzzle* generatePuzzle(T_PuzzleObjectList objects, T_PuzzleEventList events, T_PuzzleRuleList rules);

	PuzzleRelation* __simple_generateRelation(T_PuzzleNodeList nodes);

	PuzzleRelation* generateRelation(T_PuzzleNodeList nodes, T_PuzzleRuleList rules);

	T_PuzzleNodeList generateNodes(T_PuzzleObjectList objects);

	void setNumberNodes(int n);
	int getNumberNodes();

private:

	bool _checkCreatesCircularDependency(T_PuzzleNodePair P, PuzzleRelation* R);
	bool _checkCreatesExclusiveDependency(T_PuzzleNodePair P, PuzzleRelation* R);
	bool _findNodeSequential(PuzzleNode* N, T_PuzzleNodePair start, PuzzleRelation* R);
	bool _checkEquality(PuzzleNode *N1, PuzzleNode *N2);
	bool _checkMetaEqualOccurance(T_PuzzleNodePair P, PuzzleRelation *R);


	bool _checkCompatibilityBasicRules(PuzzleNode* S, PuzzleNode* N, PuzzleRelation* R);
	bool _checkCompatibilityCustomRules(T_PuzzleNodeList nodes, PuzzleNode *S, PuzzleNode* N, PuzzleRelation* R, T_PuzzleRuleList rules);

	bool _checkCompatibilityRuleType__AFTER(T_PuzzleNodeList nodes, PuzzleNode* S, PuzzleNode *N, PuzzleRelation* R, PuzzleRule rule, bool isStrict);
	bool _checkCompatibilityRuleType__BEFORE(T_PuzzleNodeList nodes, PuzzleNode* S, PuzzleNode *N, PuzzleRelation* R, PuzzleRule rule, bool isStrict);

	T_PuzzleNodeList _filterCompatibleNodes(PuzzleNode* N, PuzzleRelation *R, T_PuzzleNodeList nodes, T_PuzzleRuleList rules);

	void _initializeActivePropertyOnNodes(Puzzle* P);

	void _cleanupNodes(Puzzle* P);
	void _removeNodeFromList(PuzzleNode*N, T_PuzzleNodeList &nodes);
	int NUM_NODES = 0;
};

