#pragma once

#include "Node.h"
#include "Event.h"
#include "Relation.h"
#include "UpdateListener.h"

namespace PPG
{
	using NodeVec = Vec<Ptr<Node>>;
	using EventVec = Vec<Ptr<Event>>;

	class PPG_EXPORT Puzzle
	{
	public:
		void addNode(Ptr<Node> n, bool isRelevant);
		void setRelation(Relation O);
		void setNodes(NodeVec ns);

		void handleEvent(Event e);

		void setUpdateListener(UpdateListener* PUL);

		NodeVec& getNodes();
		Relation& getRelation();

		Str getSimpleTextualRepresentation() const;
		Str getExtendedTextualRepresentation() const;
		Str getTextualEnvironmentDescription() const;

		Vec<GraphNode*> getGraphRepresentation() const;

	private:
		NodeVec nodes;
		Relation relation;

		UpdateListener* updateListener = nullptr;

		bool canNodeHandleEvent(const Ptr<Node>& N, Event E) const;
		bool isNodeCompatible(const Ptr<Node>& N, Event E) const;
		void updateNodeProperties(Ptr<Node>& N);
		void checkPuzzleCompletion() const;
		void onPuzzleCompleted() const;

	};

}
