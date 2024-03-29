#pragma once

#include "core/Core.h"
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

		Rule(const Rule& other, EPuzzleRuleType t)
			: m_lhsObj{ other.m_lhsObj }, m_lhsState{ other.m_lhsState }, m_rhsObj{ other.m_rhsObj }, m_rhsState{ other.m_rhsState }, m_type{ t }
		{}

	public:

		void setRuleType(EPuzzleRuleType t) { m_type = t; };

		EPuzzleRuleType getRuleType() { return m_type; };

		const Object& getLeftHandSideObject() const { return *m_lhsObj; };
		const State getLeftHandSideState() const { return m_lhsState; };

		const Object& getRightHandSideObject() const { return *m_rhsObj; };
		const State getRightHandSideState() const { return m_rhsState; };

		Str getTextualRepresentation();


	private:
		EPuzzleRuleType m_type;

		Ptr<Object> m_lhsObj;
		State m_lhsState;

		Ptr<Object> m_rhsObj;
		State m_rhsState;

		Str getRuleTypeString();

	};

}
