/*
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SOGI_HEADER
#define SOGI_HEADER

#include "SOGITypes.h"
#include "stack/stack.h"

#ifdef WIN32
    #include <windows.h>
    #include <io.h>
    #include <share.h>
#endif

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>     /* for _O_TEXT and _O_BINARY */
#include <errno.h>     /* for EINVAL */
#include <sys\stat.h>  /* for _S_IWRITE */


// if null isnt defined define it as 0
#ifndef NULL
    #define NULL 0
#endif

// we only really care about the file name, and not its full path, so strip it down.
#define _FILE_NAME_ ((strrchr(__FILE__, '\\') ?: __FILE__ - 1) + 1)

// define the maximum stack trace depth
#define MAXSTACKTRACE 10

// Main Sam Oates Games Include Singlton Class
class CSOGI {
private:

	std::string				m_logPath;

private:
							//! Class constructor
							CSOGI();

							//! Copy constructor
							CSOGI(CSOGI const&);

							//! Assignment operator
	void					operator=(CSOGI const&);

                            //! Returns an array of char arrays of the call stack (depth = MAXSTACKTRACE)
	const char              **GetCallStack();

public:
							//! Get the instance of the singlton
	static CSOGI&			GetInstance()
                            {
                                static CSOGI instance;
                                return instance;
                            }

                            //! Create a console window and redirect output to it
    void                    CreateConsoleWindow();

                            //! Logs an error message to the log file
    void                    LogError(
                                const char *errorMessage,                 //!< The error message to log
                                const char *file = "",                    //!< The file where the method was called
                                int line = 0                              //!< The line number where the method was called
                            );

                            //! Shows an error message to the user
    void                    ShowError(
                                const char *errorMessage,                 //!< The error message to log
                                const char *file = "",                    //!< The file where the method was called
                                int line = 0                              //!< The line number where the method was called
                            );

                            //! Shows (platform dependant) and Logs an error message to the log file
    void                    ShowAndLogError(
                                const char *errorMessage,                 //!< The error message to log
                                const char *file = "",                    //!< The file where the method was called
                                int line = 0                              //!< The line number where the method was called
                            );

							//! Returns if a given number is almost another value
	const bool				IsAlmost(
								float x,
								float value,
								float varience = 1.0f
							);
							
};

/**
	Error Logging Macros
*/
#if defined(_DEBUG1)
	#define LOGERROR(errorMessage) \
		CSOGI::GetInstance().LogError(errorMessage, _FILE_NAME_, __LINE__);

	#define SHOWERROR(errorMessage) \
		CSOGI::GetInstance().ShowError(errorMessage, _FILE_NAME_, __LINE__);

	#define SHOWLOGERROR(errorMessage) \
		CSOGI::GetInstance().ShowAndLogError(errorMessage, _FILE_NAME_, __LINE__);
#else
	#define LOGERROR(errorMessage) \
		do { } while (false);

	#define SHOWERROR(errorMessage) \
		do { } while (false);

	#define SHOWLOGERROR(errorMessage) \
		do { } while (false);
#endif

/**
	Custom Assert
*/

#if defined(_DEBUG1)
    #if defined(WIN32)
        #define ASSERT(condition, message) \
        do { \
            if (!(condition)) { \
                std::stringstream msg; \
                msg << "Assert Failed: \"" #condition "\"\n In " << _FILE_NAME_ \
                    << "(" << __LINE__ << ")\n \"" << message << "\"\nBreak into debugger?"; \
                if (MessageBox(NULL, msg.str().c_str(), "Assert Failed", MB_YESNO | MB_ICONERROR) == IDYES) { \
                    DebugBreak(); \
                } \
            } \
        } while (false)
    #else
        #define ASSERT(condition, message) do { } while (false);
	#endif
#else
	#define ASSERT(condition, message) do { } while (false);
#endif

/**
	Function prototypes helpers
*/

/*!
 * \brief delete memory and null the pointer
 */
template<typename T>
void SafeDelete(T*& ptr) {
	if (ptr)
	{
		delete ptr;
		ptr = NULL;
	}
}

/*!
 * \brief delete memory array and null the pointer
 */
template<typename T>
void SafeArrayDelete(T*& ptr) {
	if (ptr)
	{
		delete[] ptr;
		ptr = NULL;
	}
}

/*!
 * \brief destroy a window and null the pointer
 */
template<typename T>
void SafeDestroyWindow(T*& ptr) {
	if (ptr)
	{
		DestroyWindow(ptr);
		ptr = NULL;
	}
}

/*!
 * \brief release and null a pointer
 */
template<typename T>
void SafeRelease(T*& ptr) {
	if (ptr)
	{
		ptr->Release();
		ptr = NULL;
	}
}

/*!
 * \brief release and delete, then null a pointer
 */
template<typename T>
void SafeReleaseDelete(T*& ptr) {
	if (ptr)
	{
		ptr->Release();
		delete ptr;
		ptr = NULL;
	}
}

#endif
