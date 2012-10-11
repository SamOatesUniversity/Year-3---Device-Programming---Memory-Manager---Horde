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

/*
 * \brief Class constructor
*/
CSOGI::CSOGI()
{

}

/*
 * \brief Returns an array of char arrays of the call stack (depth = MAXSTACKTRACE)
*/
const char **CSOGI::GetCallStack()
{
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
        str.copy(line, size);
        line[size] = 0;

        stackTrace[stackIndex] = line;

        stackIndex++;
        ++frame;
    }

    return stackTrace;
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
    logFile.open("SOGI-ErrorLog.log", std::ios::out | std::ios::app);

    // write our error messgae
    logFile << "[" << file << " (" << line << ")] " << errorMessage << "\n";

    // close the file as we have written to it
    logFile.close();

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
    const char **callStack = GetCallStack();

#if defined(WIN32)

    for (int stackIndex = 0; stackIndex < MAXSTACKTRACE; ++stackIndex)
    {
        LogError(callStack[stackIndex]);
    }

#else

#endif

    SafeArrayDelete(callStack);
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
    // output the error to the user
    ShowError(errorMessage, file, line);

    // log the error to file
    LogError(errorMessage, file, line);
}
