#include "PuzzleObject.h"

namespace PPG
{
	PuzzleObject::PuzzleObject()
	{
		this->objectName = "DefaultObject";
		this->templateName = this->objectName;
	}

	PuzzleObject::PuzzleObject(std::string name)
	{
		this->objectName = name;
		this->templateName = name;
	}


	PuzzleObject::~PuzzleObject()
	{
	}

	std::string PuzzleObject::getObjectName() const {
		return this->objectName;
	}

	PuzzleState PuzzleObject::getCurrentState() {
		return this->currentState;
	}


	void PuzzleObject::setStateTransition(StateTransition F)
	{
		this->stateTransition = F;
	}

	StateTransition PuzzleObject::getStateTransition()
	{
		return this->stateTransition;
	}

	void PuzzleObject::setCurrentState(PuzzleState Sc)
	{
		this->currentState = Sc;
	}



	std::vector<PuzzleState> PuzzleObject::getReachableStates()
	{
		std::vector<PuzzleState> reachables;
		reachables = this->stateTransition.getReachableStates();

		return reachables;
	}

	IPuzzleObjectMetaData PuzzleObject::getMetaData()
	{
		return this->metaData;
	}

	void PuzzleObject::setMetaData(IPuzzleObjectMetaData MD)
	{
		this->metaData = MD;
	}

	std::string PuzzleObject::getTemplateName() const
	{
		return this->templateName;
	}

	void PuzzleObject::setTemplateName(std::string tName)
	{
		this->templateName = tName;
	}

	bool PuzzleObject::isTemplateObject() const
	{
		return this->bIsTemplate;
	}

	void PuzzleObject::setIsTemplateObject(bool bTemplate)
	{
		this->bIsTemplate = bTemplate;
	}

	bool PuzzleObject::sameTemplateAs(const PuzzleObject& o2)
	{

		std::string id1 = this->isTemplateObject() ? this->getTemplateName() : this->getObjectName();
		std::string id2 = o2.isTemplateObject() ? o2.getTemplateName() : o2.getObjectName();

		return id1 == id2;
	}


	std::string PuzzleObject::getTextualRepresentation()
	{
		std::string out = "";
		out += "Gameobject: " + this->objectName + "\n";
		out += " > StateTransition: \n";
		out += this->stateTransition.getTextualOutput();
		out += " > CurrentState: \n";
		out += "  - " + this->currentState.getStateName();
		out += "\n";
		out += " > Reachable States: \n";
		out += getReachableStatesTextualRepresentation();
		out += "\n";
		return out;
	}


	std::string PuzzleObject::getReachableStatesTextualRepresentation()
	{
		std::vector<PuzzleState> reachables = getReachableStates();
		std::string out = "";
		for (std::vector<PuzzleState>::iterator vecIt = reachables.begin(); vecIt != reachables.end(); ++vecIt) {
			out += "  - " + vecIt->getStateName() + "\n";
		}
		out += "\n";
		return out;
	}

}
