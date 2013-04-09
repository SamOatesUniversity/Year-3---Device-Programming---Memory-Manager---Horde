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

#include "SOGI.h"

#include <sstream>
#include <ctime>

CSOGI CSOGI::instance;

/*
 * \brief Class constructor
*/
CSOGI::CSOGI()
{
	time_t t = time(0);   // get time now
    struct tm now;
#ifdef _MSC_VER
    localtime_s(&now, &t);
#else
    now = *localtime(&t);
#endif

	std::stringstream date;
	date << now.tm_mday << '-' << (now.tm_mon + 1) << '-' << (now.tm_year + 1900) << " " << now.tm_hour << "-" << now.tm_min;
	m_logPath = "./logs/SOGI-Log-" + date.str() + ".log";

	system("mkdir logs");
}

/*
 * \brief Returns an array of char arrays of the call stack (depth = MAXSTACKTRACE)
*/
const char **CSOGI::GetCallStack()
{
#ifdef ENABLE_STACK
    dbg::stack s(MAXSTACKTRACE);

    const char **stackTrace = new const char*[MAXSTACKTRACE];

    int stackIndex = 0;
    std::list<dbg::stack_frame>::const_iterator frame = s.begin();
    while (frame != s.end())
    {
        dbg::stack_frame f = *frame;

        std::stringstream strbuf;
        strbuf << f.instruction << ": " << f.function << " in " << f.module;

        std::string str = strbuf.str();

        const unsigned int size = str.length();
        char *line = new char[size];
        //str.copy(line, size);
        line[size] = 0;

        stackTrace[stackIndex] = line;

        stackIndex++;
        ++frame;
    }

    return stackTrace;
#else
    return NULL;
#endif
}

/*
 * \brief Log a message to a file, taking into acount line number and source file
*/
void CSOGI::LogError(
        const char *errorMessage,                       //!< The error message to log
        const char *file,                               //!< The file where the method was called
        int line                                        //!< The line number where the method was called
    )
{
    // open our log file for writting
    std::ofstream logFile;
	logFile.open(m_logPath.c_str(), std::ios::out | std::ios::app);

	if (logFile.is_open())
	{
		// write our error messgae
		logFile << "[" << file << " (" << line << ")] - " << errorMessage << "\n";

		// close the file as we have written to it
		logFile.close();
	}
}

/*
 * \brief Shows an error message to the user
*/
void CSOGI::ShowError(
        const char *errorMessage,                       //!< The error message to log
        const char *file,                               //!< The file where the method was called
        int line                                        //!< The line number where the method was called
    )
{
    std::cout << "[" << file << "(" << line << ")] " << errorMessage << "\n";
}

/*
 * \brief Shows (platform dependant) and Logs an error message to the log file
*/
void CSOGI::ShowAndLogError(
        const char *errorMessage,                       //!< The error message to log
        const char *file,                               //!< The file where the method was called
        int line                                        //!< The line number where the method was called
    )
{
    // Output a call stack
    const char **callStack = GetCallStack();

	if (callStack != NULL)
	{
		for (int stackIndex = 2; stackIndex < MAXSTACKTRACE; ++stackIndex)
		{
			if (callStack[stackIndex] != NULL) LogError(callStack[stackIndex]);
		}

		SafeArrayDelete(callStack);
	}

    // log the error to file
    LogError(errorMessage, file, line);
}

/*
 * \brief Create a console window and redirect output to it
*/
void CSOGI::CreateConsoleWindow()
{
#ifdef WIN32
    AllocConsole();

    CONSOLE_SCREEN_BUFFER_INFO coninfo;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
    coninfo.dwSize.Y = 20;
    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);

    long lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
    int hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

    FILE *fp = _fdopen( hConHandle, "w" );
    *stdout = *fp;

    setvbuf( stdout, NULL, _IONBF, 0 );
    lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
    hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
    fp = _fdopen( hConHandle, "r" );

    *stdin = *fp;
    setvbuf( stdin, NULL, _IONBF, 0 );
    lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
    hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
    fp = _fdopen( hConHandle, "w" );

    *stderr = *fp;
    setvbuf( stderr, NULL, _IONBF, 0 );
#endif
}

//! Returns if a given number is almost another value
const bool CSOGI::IsAlmost(
		float x,
		float value,
		float varience
	)
{
	if (value + varience < x)
		return false;

	if (value - varience > x)
		return false;

	return true;
}
