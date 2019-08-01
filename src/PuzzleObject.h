#pragma once

#include <string>
#include <vector>
#include "PuzzleState.h"
#include "StateTransition.h"
#include "IPuzzleObjectMetaData.h"

class PuzzleObject
{
public:
	PuzzleObject();
	PuzzleObject(std::string name);
	~PuzzleObject();
	std::string getObjectName() const;
	PuzzleState getCurrentState();
	void setStateTransition(StateTransition F);
	StateTransition getStateTransition();
	void setCurrentState(PuzzleState Sc);
	std::string getTextualRepresentation();
	std::vector<PuzzleState> getReachableStates();

	IPuzzleObjectMetaData getMetaData();
	void setMetaData(IPuzzleObjectMetaData MD);

	std::string getTemplateName() const;
	void setTemplateName(std::string tName);

	bool isTemplateObject() const;
	void setIsTemplateObject(bool bTemplate);

	bool operator==(const PuzzleObject& o2);

	bool sameTemplateAs(const PuzzleObject& o2);

private: 
	std::string objectName;
	PuzzleState currentState;
	StateTransition stateTransition;
	std::string getReachableStatesTextualRepresentation();
	IPuzzleObjectMetaData metaData;
	bool bIsTemplate = false;
	std::string templateName;
};

