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
		~Object();
		std::string getObjectName() const;
		State getCurrentState();
		void setStateTransition(StateTransition F);
		StateTransition getStateTransition();
		void setCurrentState(State Sc);
		std::string getTextualRepresentation();
		std::vector<State> getReachableStates();

		ObjectMetaData getMetaData();
		void setMetaData(ObjectMetaData MD);

		std::string getTemplateName() const;
		void setTemplateName(std::string tName);

		bool isTemplateObject() const;
		void setIsTemplateObject(bool bTemplate);

		bool operator==(const Object& o2);

		bool sameTemplateAs(const Object& o2);

	private:
		std::string objectName;
		State currentState;
		StateTransition stateTransition;
		std::string getReachableStatesTextualRepresentation();
		ObjectMetaData metaData;
		bool bIsTemplate = false;
		std::string templateName;
	};
}


