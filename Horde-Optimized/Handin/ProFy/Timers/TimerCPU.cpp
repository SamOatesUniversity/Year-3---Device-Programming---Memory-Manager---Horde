#include "TimerCPU.h"

/*
*	\brief	Class constructor
*/
TimerCPU::TimerCPU()
{
#ifdef _MSC_VER
	m_startTime = 0;
	m_frequency = 0.0;
#else

#endif
}

/*
*	\brief	Class copy constructor
*/
TimerCPU::TimerCPU(
		TimerCPU &instance						//!< Instance to instanciate from
	)
{

}

/*
*	\brief	Class destructor
*/
TimerCPU::~TimerCPU()
{

}

/*
*	\brief	Create the timer, return false on failure to create, true on success
*/
bool TimerCPU::Create(
		std::string &name						//!< The name of the timer
	)
{
	m_name = name;

#ifdef _MSC_VER
	LARGE_INTEGER li;
	if(!QueryPerformanceFrequency(&li))
		return false;
#else
    timeval ti;
    if (gettimeofday(&ti, NULL) != 0)
        return false;
#endif

    m_running = false;
    m_ended = false;

	return true;
}

/*
*	\brief	Release all allocated timer data
*/
void TimerCPU::Destroy()
{

}

/*
*	\brief	Return the last time it took the timer to get from start to end
*/
float TimerCPU::GetLastTime()
{
	return m_lastTime;
}

/*
*	\brief	Start the timer
*/
void TimerCPU::Start()
{
	if (m_running == true)
		return;

    #ifdef _MSC_VER
        LARGE_INTEGER li;
        if (QueryPerformanceFrequency(&li))
        {
            m_frequency = static_cast<double>(li.QuadPart) / 0.001;
            QueryPerformanceCounter(&li);
            m_startTime = li.QuadPart;

            m_running = true;
            m_ended = false;
        }
    #else
        if (gettimeofday(&m_startTime, NULL) == 0)
        {
            m_running = true;
            m_ended = false;
        }
	#endif
}

/*
*	\brief	End the timer
*/
void TimerCPU::End()
{
	if (m_ended == true)
		return;

    #ifdef _MSC_VER
        LARGE_INTEGER li;
        QueryPerformanceCounter(&li);
        double timeDifference = static_cast<double>(li.QuadPart - m_startTime) / m_frequency;
        m_lastTime = static_cast<float>(timeDifference * 1000000.0f);
	#else
        timeval endTime;
        gettimeofday(&endTime, NULL);

        double t1 = m_startTime.tv_sec+(m_startTime.tv_usec/1000000.0);
        double t2 = endTime.tv_sec+(endTime.tv_usec/1000000.0);

        const float timeDifference = t2 - t1;
        m_lastTime = timeDifference * 1000.0f;
	#endif

    // store the time in the list of times
    m_time.push_back(m_lastTime);

    m_running = false;
    m_ended = true;
}
