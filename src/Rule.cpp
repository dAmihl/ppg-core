#include "Rule.h"


namespace PPG
{
	Str Rule::getTextualRepresentation()
	{

		Str lhS;
		if (getLeftHandSideState() == STATE_ANY) lhS = "{}";
		else lhS = getLeftHandSideState().getName();

		Str rhS;
		if (getRightHandSideState() == STATE_ANY) rhS = "{}";
		else rhS = getRightHandSideState().getName();

		Str out = ">> Rule (" + getLeftHandSideObject().getObjectName() + " | " + lhS + ") " + getRuleTypeString() + " (" + getRightHandSideObject().getObjectName() + " | " + rhS + ") \n";
		return out;
	}

	Str Rule::getRuleTypeString()
	{
		Str out;
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
