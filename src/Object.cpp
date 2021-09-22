#include "Object.h"

namespace PPG
{
	Object::Object()
	{
		this->objectName = "DefaultObject";
		this->templateName = this->objectName;
	}

	Object::Object(std::string name)
	{
		this->objectName = name;
		this->templateName = name;
	}


	Object::~Object()
	{
	}

	std::string Object::getObjectName() const {
		return this->objectName;
	}

	State Object::getCurrentState() {
		return this->currentState;
	}


	void Object::setStateTransition(StateTransition F)
	{
		this->stateTransition = F;
	}

	StateTransition Object::getStateTransition()
	{
		return this->stateTransition;
	}

	void Object::setCurrentState(State Sc)
	{
		this->currentState = Sc;
	}



	std::vector<State> Object::getReachableStates()
	{
		std::vector<State> reachables;
		reachables = this->stateTransition.getReachableStates();

		return reachables;
	}

	ObjectMetaData Object::getMetaData()
	{
		return this->metaData;
	}

	void Object::setMetaData(ObjectMetaData MD)
	{
		this->metaData = MD;
	}

	std::string Object::getTemplateName() const
	{
		return this->templateName;
	}

	void Object::setTemplateName(std::string tName)
	{
		this->templateName = tName;
	}

	bool Object::isTemplateObject() const
	{
		return this->bIsTemplate;
	}

	void Object::setIsTemplateObject(bool bTemplate)
	{
		this->bIsTemplate = bTemplate;
	}

	bool Object::sameTemplateAs(const Object& o2)
	{

		std::string id1 = this->isTemplateObject() ? this->getTemplateName() : this->getObjectName();
		std::string id2 = o2.isTemplateObject() ? o2.getTemplateName() : o2.getObjectName();

		return id1 == id2;
	}


	std::string Object::getTextualRepresentation()
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


	std::string Object::getReachableStatesTextualRepresentation()
	{
		std::vector<State> reachables = getReachableStates();
		std::string out = "";
		for (std::vector<State>::iterator vecIt = reachables.begin(); vecIt != reachables.end(); ++vecIt) {
			out += "  - " + vecIt->getStateName() + "\n";
		}
		out += "\n";
		return out;
	}

}
