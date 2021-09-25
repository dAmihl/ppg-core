#pragma once

#include "core/Node.h"
#include "Event.h"
#include "core/Relation.h"
#include "UpdateListener.h"
#include "Context.h"

namespace PPG
{
	using NodeVec = Vec<Ptr<Node>>;
	using EventVec = Vec<Ptr<Event>>;

	class PPG_EXPORT Puzzle
	{
	public:
		void addNode(Ptr<Node> n);
		void setRelation(Relation O);
		void setNodes(NodeVec ns);
		void setContext(Context c);

		void handleEvent(Event e);

		void setUpdateListener(UpdateListener* PUL);

		NodeVec& getNodes();
		Relation& getRelation();
		Context& getContext();

		Str getSimpleTextualRepresentation() const;
		Str getExtendedTextualRepresentation() const;
		Str getTextualEnvironmentDescription() const;

		Vec<GraphNode*> getGraphRepresentation() const;

	private:
		NodeVec nodes;
		Relation relation;
		Context context;

		UpdateListener* updateListener = nullptr;

		bool canNodeHandleEvent(const Ptr<Node>& N, Event E) const;
		bool isNodeCompatible(const Ptr<Node>& N, Event E) const;
		void updateNodeProperties(Ptr<Node>& N);
		void checkPuzzleCompletion() const;
		void onPuzzleCompleted() const;

	};

}
