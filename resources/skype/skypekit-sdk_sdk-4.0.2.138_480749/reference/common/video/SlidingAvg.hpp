/*
 * SlidingAvg.hpp
 *
 *  Created on: Oct 10, 2010
 *      Author: lauri
 */

#ifndef SLIDINGAVG_HPP_
#define SLIDINGAVG_HPP_

template <class ValueType, int depth>
class SlidingAvg {
	int m_Index;
	ValueType m_Values[depth];
	ValueType m_ValuesSum;
	bool m_Valid;
public:
	SlidingAvg() : m_Index(0), m_Valid(false) {
	}
	~SlidingAvg() {
	}
	void Update(ValueType value) {
		if (m_Valid) {
			m_ValuesSum -= m_Values[m_Index];
		}
		m_Values[m_Index] = value;
		m_ValuesSum += value;
		m_Index++;
		if (m_Index == depth) {
			m_Valid = true;
			m_Index = 0;
		}
	}
	bool GetAvg(ValueType & avg) {
		if (m_Valid) {
			avg = m_ValuesSum/depth;
			return true;
		} else {
			return false;
		}
	}
};

#endif /* SLIDINGAVG_HPP_ */
