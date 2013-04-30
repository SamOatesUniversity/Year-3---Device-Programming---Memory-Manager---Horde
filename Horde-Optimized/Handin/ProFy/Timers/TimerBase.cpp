#include "TimerBase.h"

/*
*	\brief	Class constructor
*/
TimerBase::TimerBase() : m_lastTime(0)
{

}

/*
*	\brief	Class copy constructor
*/
TimerBase::TimerBase(
		TimerBase &instance										//!< Instance to instanciate from
	)
{

}

/*
*	\brief	Class destructor
*/
TimerBase::~TimerBase()
{

}

/*
*	\brief	Return the list of timer results
*/
std::vector<float> &TimerBase::GetResults()
{
	return m_time;
}

/*
*	\brief	Get the name of the timer
*/
std::string &TimerBase::GetName()
{
	return m_name;
}

/*
*	\brief
*/
void TimerBase::AddFakeResult(
		float time
	)
{
	m_time.push_back(time);
}

/*
*	\brief
*/
const float TimerBase::CalculateAverageTime() const
{
	if (m_time.empty())
	{
		return 0.0f;
	}

	float totalTime = 0.0f;
	const unsigned int noofTime = m_time.size();
	for (unsigned int timeIndex = 0; timeIndex < noofTime; ++timeIndex)
	{
		totalTime += m_time[timeIndex];
	}

	return totalTime / m_time.size();
}
