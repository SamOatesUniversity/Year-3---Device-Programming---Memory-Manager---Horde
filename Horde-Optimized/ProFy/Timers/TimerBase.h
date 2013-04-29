#ifndef PROFY_HEADER_TIMERBASE
#define PROFY_HEADER_TIMERBASE

#include <vector>
#include <string>

class TimerBase {

protected:

	friend class ProFy;
	friend class OutputHtml;

protected:

	float									m_lastTime;									//!< The last known timer length
	std::vector<float>						m_time;										//!< A list of all the times this timer has executed

	bool									m_running;									//!< Is the timer running
	bool									m_ended;									//!< Has the timer ended

	std::string								m_name;										//!< The name of the timer, used for output

protected:

											//! Class Constructor
											TimerBase();

											//! Class Copy Constructor
											TimerBase(
												TimerBase &instance						//!< Instance to instanciate from
											);

											//! Class Destructor
											~TimerBase();

protected:
											//! Create the timer, return false on failure to create, true on success
	virtual bool							Create(
												std::string &name						//!< The name of the timer
											) = 0;

											//! Release all allocated timer data
	virtual void							Destroy() = 0;

											//! Return the last time it took the timer to get from start to end
	virtual float							GetLastTime() = 0;

											//! Return the list of timer results
	std::vector<float>						&GetResults();

											//! Get the name of the timer
	std::string								&GetName();

											//! Start the timer
	virtual void							Start() = 0;

											//! End the timer
	virtual void							End() = 0;

											//!
	void									AddFakeResult(
												float time
											);
				
											//! 
	const float								CalculateAverageTime() const;
};

#endif
