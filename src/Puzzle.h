#pragma once

#include "Node.h"
#include "Event.h"
#include "Relation.h"
#include "UpdateListener.h"

namespace PPG
{
	using NodeVec = Vec<Node*>;
	using EventVec = Vec<Ptr<Event>>;

	class PPG_EXPORT Puzzle
	{
	public:
		void addNode(Node* n, bool isRelevant);
		void addEvent(Ptr<Event> e);
		void setRelation(Relation O);
		void setNodes(NodeVec ns);

		void handleEvent(Event e);

		void setUpdateListener(UpdateListener* PUL);

		NodeVec getNodes() const;
		NodeVec getRelevantNodes() const;
		EventVec& getEvents();
		Relation getRelation() const;

		Str getSimpleTextualRepresentation() const;
		Str getExtendedTextualRepresentation() const;
		Str getTextualEnvironmentDescription() const;

		Vec<GraphNode*> getGraphRepresentation() const;

	private:
		NodeVec nodes;
		NodeVec relevantNodes;
		EventVec events;
		Relation relation;

		UpdateListener* updateListener = nullptr;

		bool canNodeHandleEvent(const Node* N, Event E) const;
		bool isNodeCompatible(const Node* N, Event E) const;
		void updateNodeProperties(Node* N);
		void checkPuzzleCompletion() const;
		void onPuzzleCompleted() const;

	};

}
