#include "PuzzleRule.h"


PuzzleRule::PuzzleRule(PuzzleObject* leftO, PuzzleState* leftS, PuzzleObject* rightO, PuzzleState* rightS, E_PuzzleRuleType t)
{
	this->_lhsObj = leftO;
	this->_lhsState = leftS;
	this->_rhsObj = rightO;
	this->_rhsState = rightS;
	this->type = t;
}


PuzzleRule::~PuzzleRule()
{
}

void PuzzleRule::setRuleType(E_PuzzleRuleType t)
{
	this->type = t;
}

PuzzleRule::E_PuzzleRuleType PuzzleRule::getRuleType()
{
	return this->type;
}

PuzzleObject * PuzzleRule::getLeftHandSideObject()
{
	return this->_lhsObj;
}

PuzzleState * PuzzleRule::getLeftHandSideState()
{
	return this->_lhsState;
}

PuzzleObject * PuzzleRule::getRightHandSideObject()
{
	return this->_rhsObj;
}

PuzzleState * PuzzleRule::getRightHandSideState()
{
	return this->_rhsState;
}

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
		case E_PuzzleRuleType::AFTER: out = "AFTER"; break;
		case E_PuzzleRuleType::BEFORE: out = "BEFORE"; break;
		case E_PuzzleRuleType::STRICT_AFTER: out = "STRICTAFTER"; break;
		case E_PuzzleRuleType::STRICT_BEFORE: out = "STRICTBEFORE"; break;
		default: out = "NOT_FOUND";
	}
	return out;
}
