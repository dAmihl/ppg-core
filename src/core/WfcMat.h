#pragma once

#include "Core.h"

namespace PPG
{

	enum class EWfcCellState
	{
		FREE,
		USED,
		NOT,
		NTNOT
	};

	class WfcMat
	{

	public:
		WfcMat(size_t size) 
			: m_size(size), 
			m_data{ m_size * m_size, EWfcCellState::FREE }
		{}

		// default dtor
		~WfcMat() = default;
		
		// default copy and mv ctor
		WfcMat(const WfcMat& other) = default;
		WfcMat(WfcMat&& mv) = default;

		// default copy and mv assign
		WfcMat& operator=(const WfcMat& other) = default;
		WfcMat& operator=(WfcMat&& other) = default;


		EWfcCellState operator[](size_t ix) = delete;
		
		EWfcCellState at(size_t x, size_t y) const
		{
			return m_data[x * m_size + y];
		}

		void set(size_t x, size_t y, EWfcCellState val)
		{
			m_data[x * m_size + y] = val;
		}


	private:
		const size_t m_size;
		Vec<EWfcCellState> m_data;

	};

}

