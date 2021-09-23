#include "Object.h"

namespace PPG
{
	Object::Object() : objectName{ "DefaultObject" }, templateName{ objectName }
	{}

	Object::Object(std::string name) : objectName{ name }, templateName{ name }
	{}

	std::string Object::getObjectName() const {
		return objectName;
	}

	State* Object::getCurrentState() const {
		return currentState;
	}

	StateTransition Object::getStateTransition() const
	{
		return stateTransition;
	}

	std::vector<State*> Object::getReachableStates() const
	{
		std::vector<State*> reachables;
		reachables = stateTransition.getReachableStates();
		return reachables;
	}

	ObjectMetaData Object::getMetaData() const
	{
		return this->metaData;
	}

	std::string Object::getTemplateName() const
	{
		return this->templateName;
	}

	bool Object::isTemplateObject() const
	{
		return this->bIsTemplate;
	}

	void Object::setStateTransition(StateTransition F)
	{
		this->stateTransition = F;
	}

	void Object::setCurrentState(State* Sc)
	{
		this->currentState = Sc;
	}

	void Object::setMetaData(ObjectMetaData MD)
	{
		this->metaData = MD;
	}

	void Object::setTemplateName(std::string tName)
	{
		this->templateName = tName;
	}

	void Object::setIsTemplateObject(bool bTemplate)
	{
		this->bIsTemplate = bTemplate;
	}

	bool Object::sameTemplateAs(const Object& o2) const
	{
		std::string id1 = isTemplateObject() ? getTemplateName() : getObjectName();
		std::string id2 = o2.isTemplateObject() ? o2.getTemplateName() : o2.getObjectName();

		return id1 == id2;
	}


	std::string Object::getTextualRepresentation() const
	{
		std::string out = "";
		out += "Gameobject: " + this->objectName + "\n";
		out += " > StateTransition: \n";
		out += stateTransition.getTextualOutput();
		out += " > CurrentState: \n";
		out += "  - " + this->currentState->getStateName();
		out += "\n";
		out += " > Reachable States: \n";
		out += getReachableStatesTextualRepresentation();
		out += "\n";
		return out;
	}


	std::string Object::getReachableStatesTextualRepresentation() const
	{
		std::vector<State*> reachables = getReachableStates();
		std::string out = "";
		for (auto& vecIt: reachables) {
			out += "  - " + vecIt->getStateName() + "\n";
		}
		out += "\n";
		return out;
	}

}
