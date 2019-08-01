#pragma once

#include "PuzzleObject.h"
#include "PuzzleState.h"

class PuzzleRule
{
public:
	enum E_PuzzleRuleType {
		BEFORE,
		STRICT_BEFORE,
		AFTER,
		STRICT_AFTER
	};

	PuzzleRule(PuzzleObject* leftO, PuzzleState* leftS, PuzzleObject* rightO, PuzzleState* rightS, E_PuzzleRuleType t);
	~PuzzleRule();

	

	/*Setters*/
	void setRuleType(E_PuzzleRuleType t);

	/*Getters*/
	E_PuzzleRuleType getRuleType();

	PuzzleObject* getLeftHandSideObject();
	PuzzleState* getLeftHandSideState();
	
	PuzzleObject* getRightHandSideObject();
	PuzzleState* getRightHandSideState();

	std::string getTextualRepresentation();
	

private:
	E_PuzzleRuleType type;

	PuzzleObject* _lhsObj;
	PuzzleState* _lhsState;

	PuzzleObject* _rhsObj;
	PuzzleState* _rhsState;

	std::string getRuleTypeString();

};

