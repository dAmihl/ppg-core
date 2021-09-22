#pragma once

#include "Node.h"
#include "PuzzleEvent.h"
#include "PuzzleRelation.h"
#include "PuzzleUpdateListener.h"

namespace PPG
{
	using NodeVec = Vec<Node*>;
	using EventVec = Vec<PuzzleEvent*>;

	class Puzzle
	{
	public:
		void addNode(Node* n, bool isRelevant);
		void addEvent(PuzzleEvent* e);
		void setRelation(PuzzleRelation O);
		void setNodes(NodeVec nodes);

		void handleEvent(PuzzleEvent e);

		void setUpdateListener(PuzzleUpdateListener* PUL);

		NodeVec getNodes();
		NodeVec getRelevantNodes();
		EventVec getEvents();
		PuzzleRelation getRelation();

		std::string getSimpleTextualRepresentation();
		std::string getExtendedTextualRepresentation();
		std::string getTextualEnvironmentDescription();

		Vec<PuzzleGraphNode*> getGraphRepresentation();

	private:
		NodeVec nodes;
		NodeVec relevantNodes;
		EventVec events;
		PuzzleRelation relation;

		PuzzleUpdateListener* updateListener = NULL;

		/*
		void doSetNodeActive(PuzzleNode* M);
		void doCheckNodeComplete(PuzzleNode* M);
		void doSetNodeIncomplete(PuzzleNode* M);
		*/
		bool canNodeHandleEvent(Node* N, PuzzleEvent E);
		bool isNodeCompatible(Node* N, PuzzleEvent E);
		void updateNodeProperties(Node* N);
		void checkPuzzleCompletion();
		void onPuzzleCompleted();

	};

}
