#pragma once

#include "Core.h"
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

		Rule(const Ptr<Object> leftO, const State leftS, const Ptr<Object> rightO, const State rightS, EPuzzleRuleType t)
			: m_lhsObj{ leftO }, m_lhsState{ leftS }, m_rhsObj{ rightO }, m_rhsState{ rightS }, m_type{ t }
		{};

	public:

		void setRuleType(EPuzzleRuleType t) { m_type = t; };

		EPuzzleRuleType getRuleType() { return m_type; };

		const Ptr<Object> getLeftHandSideObject() const { return m_lhsObj; };
		const State getLeftHandSideState() const { return m_lhsState; };

		const Ptr<Object> getRightHandSideObject() const { return m_rhsObj; };
		const State getRightHandSideState() const { return m_rhsState; };

		Str getTextualRepresentation();


	private:
		EPuzzleRuleType m_type;

		const Ptr<Object> m_lhsObj;
		const State m_lhsState;

		const Ptr<Object> m_rhsObj;
		const State m_rhsState;

		Str getRuleTypeString();

	};

}
