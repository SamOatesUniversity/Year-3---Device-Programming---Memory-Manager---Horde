#ifndef PROFY_HEADER_TIMERCPU
#define PROFY_HEADER_TIMERCPU

#include "TimerBase.h"

#ifdef _MSC_VER
    #include <windows.h>
#else
    #include <sys/time.h>
#endif

class TimerCPU : public TimerBase {

private:

#ifdef _MSC_VER
	__int64									m_startTime;
	double									m_frequency;
#else
    timeval                                 m_startTime;
#endif

protected:

	friend class ProFy;

protected:

											//! Class Constructor
											TimerCPU();

											//! Class Copy Constructor
											TimerCPU(
												TimerCPU &instance						//!< Instance to instanciate from
											);

											//! Class Destructor
											~TimerCPU();

protected:
											//! Create the timer, return false on failure to create, true on success
	virtual bool							Create(
												std::string &name						//!< The name of the timer
											);

											//! Release all allocated timer data
	virtual void							Destroy();

											//! Return the last time it took the timer to get from start to end
	virtual float							GetLastTime();

											//! Start the timer
	virtual void							Start();

											//! End the timer
	virtual void							End();

};

#endif
