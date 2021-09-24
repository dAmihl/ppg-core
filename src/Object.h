#pragma once

#include "core/Core.h"
#include "State.h"
#include "StateTransition.h"
#include "core/ObjectMetaData.h"

namespace PPG
{
	class PPG_EXPORT Object
	{
	
	public:
		Object();
		Object(Str name);
	
	public:
		Str getObjectName() const;
		State getCurrentState() const;
		StateTransition getStateTransition() const;
		Str getTextualRepresentation() const;
		Vec<State> getReachableStates() const;
		ObjectMetaData getMetaData() const;
		Str getTemplateName() const;
		bool isTemplateObject() const;

		void setStateTransition(const StateTransition F);
		void setCurrentState(State Sc);
		void setMetaData(ObjectMetaData MD);
		void setTemplateName(Str tName);
		void setIsTemplateObject(bool bTemplate);

		bool sameTemplateAs(const Object& o2) const;

	private:
		Str objectName;
		State currentState;
		StateTransition stateTransition;
		Str getReachableStatesTextualRepresentation() const;
		ObjectMetaData metaData;
		bool bIsTemplate = false;
		Str templateName;
	};

	inline bool operator==(const Object& lhs, const Object& rhs) {
		return &lhs == &rhs;
	}

}


