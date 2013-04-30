#ifndef PROFY_HEADER_TIMEROPENGL
#define PROFY_HEADER_TIMEROPENGL

#ifdef _MSC_VER

#include "TimerBase.h"
#include <tgl.h>

class TimerOpenGL : public TimerBase {

private:

	GLuint									m_query;
	bool									m_started;
	bool									m_finished;
	bool									m_hasResult;

protected:

	friend class ProFy;

protected:

											//! Class Constructor
											TimerOpenGL();

											//! Class Copy Constructor
											TimerOpenGL(
												TimerOpenGL &instance					//!< Instance to instanciate from
											);

											//! Class Destructor
											~TimerOpenGL();

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

#endif
