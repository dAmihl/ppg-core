#include "PuzzleRule.h"



std::string PuzzleRule::getTextualRepresentation()
{

	std::string lhS;
	if (this->getLeftHandSideState() == nullptr) lhS = "{}";
	else lhS = this->getLeftHandSideState()->getStateName();

	std::string rhS;
	if (this->getRightHandSideState() == nullptr) rhS = "{}";
	else rhS = this->getRightHandSideState()->getStateName();

	std::string out = ">> PuzzleRule (" + this->getLeftHandSideObject()->getObjectName() + " | " + lhS + ") " + this->getRuleTypeString() + " (" + this->getRightHandSideObject()->getObjectName() + " | " + rhS + ") \n";
	return out;
}

std::string PuzzleRule::getRuleTypeString()
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
