#pragma once

#include "PuzzGenCore.h"
#include "Object.h"
#include "State.h"

namespace PPG
{
	class PPG_EXPORT Rule
	{
	public:
		enum class EPuzzleRuleType {
			BEFORE,
			STRICT_BEFORE,
			AFTER,
			STRICT_AFTER
		};

		Rule(Object* leftO, State* leftS, Object* rightO, State* rightS, EPuzzleRuleType t)
			: m_lhsObj{ leftO }, m_lhsState{ leftS }, m_rhsObj{ rightO }, m_rhsState{ rightS }, m_type{ t }
		{};


		void setRuleType(EPuzzleRuleType t) { m_type = t; };

		EPuzzleRuleType getRuleType() { return m_type; };

		Object* getLeftHandSideObject() { return m_lhsObj; };
		State* getLeftHandSideState() { return m_lhsState; };

		Object* getRightHandSideObject() { return m_rhsObj; };
		State* getRightHandSideState() { return m_rhsState; };

		std::string getTextualRepresentation();


	private:
		EPuzzleRuleType m_type;

		Object* m_lhsObj;
		State* m_lhsState;

		Object* m_rhsObj;
		State* m_rhsState;

		std::string getRuleTypeString();

	};

}
