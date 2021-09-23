#pragma once

#include "PuzzGenCore.h"
#include <string>
#include <vector>
#include "State.h"
#include "StateTransition.h"
#include "ObjectMetaData.h"

namespace PPG
{
	class PPG_EXPORT Object
	{
	public:
		Object();
		Object(std::string name);

		std::string getObjectName() const;
		State* getCurrentState() const;
		StateTransition getStateTransition() const;
		std::string getTextualRepresentation() const;
		std::vector<State*> getReachableStates() const;
		ObjectMetaData getMetaData() const;
		std::string getTemplateName() const;
		bool isTemplateObject() const;

		void setStateTransition(const StateTransition F);
		void setCurrentState(State* Sc);
		void setMetaData(ObjectMetaData MD);
		void setTemplateName(std::string tName);
		void setIsTemplateObject(bool bTemplate);

		bool sameTemplateAs(const Object& o2) const;

	private:
		std::string objectName;
		State* currentState;
		StateTransition stateTransition;
		std::string getReachableStatesTextualRepresentation() const;
		ObjectMetaData metaData;
		bool bIsTemplate = false;
		std::string templateName;
	};

}


