#include "Object.h"

namespace PPG
{
	Object::Object() : objectName{ "DefaultObject" }, templateName{ objectName }
	{}

	Object::Object(Str name) : objectName{ name }, templateName{ name }
	{}

	Str Object::getObjectName() const {
		return objectName;
	}

	PPG::State Object::getCurrentState() const
{
		return currentState;
	}

	StateTransition Object::getStateTransition() const
	{
		return stateTransition;
	}

	Vec<PPG::State> Object::getReachableStates() const
	{
		return stateTransition.getReachableStates();	
	}

	ObjectMetaData Object::getMetaData() const
	{
		return metaData;
	}

	Str Object::getTemplateName() const
	{
		return templateName;
	}

	bool Object::isTemplateObject() const
	{
		return bIsTemplate;
	}

	void Object::setStateTransition(StateTransition F)
	{
		stateTransition = F;
	}

	void Object::setCurrentState(State Sc)
	{
		currentState = Sc;
	}

	void Object::setMetaData(ObjectMetaData MD)
	{
		metaData = MD;
	}

	void Object::setTemplateName(Str tName)
	{
		templateName = tName;
	}

	void Object::setIsTemplateObject(bool bTemplate)
	{
		bIsTemplate = bTemplate;
	}

	bool Object::sameTemplateAs(const Object& o2) const
	{
		Str id1 = isTemplateObject() ? getTemplateName() : getObjectName();
		Str id2 = o2.isTemplateObject() ? o2.getTemplateName() : o2.getObjectName();

		return id1 == id2;
	}


	Str Object::getTextualRepresentation() const
	{
		Str out = "";
		out += "Gameobject: " + objectName + "\n";
		out += " > StateTransition: \n";
		out += stateTransition.getTextualOutput();
		out += " > CurrentState: \n";
		out += "  - " + currentState.getName();
		out += "\n";
		out += " > Reachable States: \n";
		out += getReachableStatesTextualRepresentation();
		out += "\n";
		return out;
	}


	Str Object::getReachableStatesTextualRepresentation() const
	{
		Vec<State> reachables = getReachableStates();
		Str out = "";
		for (auto& vecIt: reachables) {
			out += "  - " + vecIt.getName() + "\n";
		}
		out += "\n";
		return out;
	}

}
