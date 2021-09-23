#include "Rule.h"


namespace PPG
{
	std::string Rule::getTextualRepresentation()
	{

		std::string lhS;
		if (getLeftHandSideState() == STATE_ANY) lhS = "{}";
		else lhS = getLeftHandSideState().getName();

		std::string rhS;
		if (getRightHandSideState() == STATE_ANY) rhS = "{}";
		else rhS = getRightHandSideState().getName();

		std::string out = ">> Rule (" + getLeftHandSideObject()->getObjectName() + " | " + lhS + ") " + getRuleTypeString() + " (" + getRightHandSideObject()->getObjectName() + " | " + rhS + ") \n";
		return out;
	}

	std::string Rule::getRuleTypeString()
	{
		std::string out;
		switch (this->getRuleType()) {
		case EPuzzleRuleType::AFTER: out = "AFTER"; break;
		case EPuzzleRuleType::BEFORE: out = "BEFORE"; break;
		case EPuzzleRuleType::STRICT_AFTER: out = "STRICTAFTER"; break;
		case EPuzzleRuleType::STRICT_BEFORE: out = "STRICTBEFORE"; break;
		default: out = "NOT_FOUND";
		}
		return out;
	}

}
