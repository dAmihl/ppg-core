#pragma once

#include "Node.h"
#include "Event.h"
#include "Relation.h"
#include "UpdateListener.h"

namespace PPG
{
	using NodeVec = Vec<Node*>;
	using EventVec = Vec<Event*>;

	class PPG_EXPORT Puzzle
	{
	public:
		void addNode(Node* n, bool isRelevant);
		void addEvent(Event* e);
		void setRelation(Relation O);
		void setNodes(NodeVec nodes);

		void handleEvent(Event e);

		void setUpdateListener(UpdateListener* PUL);

		NodeVec getNodes();
		NodeVec getRelevantNodes();
		EventVec getEvents();
		Relation getRelation();

		std::string getSimpleTextualRepresentation();
		std::string getExtendedTextualRepresentation();
		std::string getTextualEnvironmentDescription();

		Vec<GraphNode*> getGraphRepresentation();

	private:
		NodeVec nodes;
		NodeVec relevantNodes;
		EventVec events;
		Relation relation;

		UpdateListener* updateListener = NULL;

		/*
		void doSetNodeActive(PuzzleNode* M);
		void doCheckNodeComplete(PuzzleNode* M);
		void doSetNodeIncomplete(PuzzleNode* M);
		*/
		bool canNodeHandleEvent(Node* N, Event E);
		bool isNodeCompatible(Node* N, Event E);
		void updateNodeProperties(Node* N);
		void checkPuzzleCompletion();
		void onPuzzleCompleted();

	};

}
