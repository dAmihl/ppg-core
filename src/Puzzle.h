#pragma once

#include "PuzzleNode.h"
#include "PuzzleEvent.h"
#include "PuzzleRelation.h"
#include "PuzzleUpdateListener.h"

class Puzzle
{
public:
	Puzzle();
	~Puzzle();

	void addNode(PuzzleNode* n, bool isRelevant);
	void addEvent(PuzzleEvent* e);
	void setRelation(PuzzleRelation O);
	void setNodes(T_PuzzleNodeList nodes);

	void handleEvent(PuzzleEvent e);

	void setUpdateListener(PuzzleUpdateListener* PUL);

	T_PuzzleNodeList getNodes();
	T_PuzzleNodeList getRelevantNodes();
	T_PuzzleEventList getEvents();
	PuzzleRelation getRelation();

	std::string getSimpleTextualRepresentation();
	std::string getExtendedTextualRepresentation();
	std::string getTextualEnvironmentDescription();

private: 
	T_PuzzleNodeList nodes;
	T_PuzzleNodeList relevantNodes;
	T_PuzzleEventList events;
	PuzzleRelation relation;

	PuzzleUpdateListener* updateListener = NULL;

	/*
	void doSetNodeActive(PuzzleNode* M);
	void doCheckNodeComplete(PuzzleNode* M);
	void doSetNodeIncomplete(PuzzleNode* M);
	*/
	bool canNodeHandleEvent(PuzzleNode* N, PuzzleEvent E);
	bool isNodeCompatible(PuzzleNode *N, PuzzleEvent E);
	void updateNodeProperties(PuzzleNode* N);
	void checkPuzzleCompletion();
	void onPuzzleCompleted();

};

