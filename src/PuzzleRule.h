#pragma once

#include "PuzzleObject.h"
#include "PuzzleState.h"

class PuzzleRule
{
public:
	enum class EPuzzleRuleType {
		BEFORE,
		STRICT_BEFORE,
		AFTER,
		STRICT_AFTER
	};

	PuzzleRule(PuzzleObject* leftO, PuzzleState* leftS, PuzzleObject* rightO, PuzzleState* rightS, EPuzzleRuleType t)
		: m_lhsObj{ leftO }, m_lhsState{ leftS }, m_rhsObj{ rightO }, m_rhsState{ rightS }, m_type{ t } 
	{};

	
	void setRuleType(EPuzzleRuleType t) { m_type = t };

	EPuzzleRuleType getRuleType() { return m_type; };

	PuzzleObject* getLeftHandSideObject() { return m_lhsObj; };
	PuzzleState* getLeftHandSideState() { return m_lhsState; };
	
	PuzzleObject* getRightHandSideObject() { return m_rhsObj; };
	PuzzleState* getRightHandSideState() { return m_rhsState; };

	std::string getTextualRepresentation();
	

private:
	EPuzzleRuleType m_type;

	PuzzleObject* m_lhsObj;
	PuzzleState* m_lhsState;

	PuzzleObject* m_rhsObj;
	PuzzleState* m_rhsState;

	std::string getRuleTypeString();

};

