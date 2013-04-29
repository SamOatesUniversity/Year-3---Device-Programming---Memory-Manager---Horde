#include "TimerOpenGL.h"

#ifdef _MSC_VER

/*
*	\brief	Class constructor
*/
TimerOpenGL::TimerOpenGL()
{
	m_query = 0;
	m_started = false;
	m_finished = false;
	m_hasResult = false;
}

/*
*	\brief	Class copy constructor
*/
TimerOpenGL::TimerOpenGL(
	TimerOpenGL &instance						//!< Instance to instanciate from
	)
{

}

/*
*	\brief	Class destructor
*/
TimerOpenGL::~TimerOpenGL()
{

}

/*
*	\brief	Create the timer, return false on failure to create, true on success
*/
bool TimerOpenGL::Create(
		std::string &name						//!< The name of the timer
	)
{
	m_name = name;

	m_query = 0;
	glGenQueries(1, &m_query);

	if (m_query == 0)
		return false;

	return true;
}

/*
*	\brief	Release all allocated timer data
*/
void TimerOpenGL::Destroy()
{

}

/*
*	\brief	Return the last time it took the timer to get from start to end
*/
float TimerOpenGL::GetLastTime()
{
	return m_lastTime;
}

/*
*	\brief	Start the timer
*/
void TimerOpenGL::Start()
{
	if (m_started == true)
	{
		return;
	}

	if (m_finished == true && m_hasResult == false)
	{
		return;
	}

	glBeginQuery(GL_TIME_ELAPSED, m_query);
	m_started = true;
	m_finished = false;
	m_hasResult = false;
}

/*
*	\brief	End the timer
*/
void TimerOpenGL::End()
{
	if (m_started == true)
	{
		glEndQuery(GL_TIME_ELAPSED);
		m_started = false;
		m_finished = true;
		m_hasResult = false;
	}	

	if (m_finished == true)
	{
		GLint resultReady = 0;
		glGetQueryObjectiv(m_query, GL_QUERY_RESULT_AVAILABLE, &resultReady);

		if (resultReady)
		{
			GLuint result = 0;
			glGetQueryObjectuiv(m_query, GL_QUERY_RESULT, &result);
			m_hasResult = true;
			m_lastTime = result * 0.000001f;
			m_time.push_back(m_lastTime);
		}
		
	}
}

#endif
