
#include "ProFy.h"

ProFy ProFy::instance;

/*
*	\brief	Class Constructor
*/
ProFy::ProFy() : m_output(NULL)
{
	m_output = new OutputHtml();
}

/*
*	\brief	Class Copy Constructor
*/
ProFy::ProFy(
		ProFy const& instance							//!< The instance to instanciate from
	)
{

}

/*
*	\brief	Class Destructor
*/
ProFy::~ProFy()
{
	std::vector<TimerBase*>::iterator timerIterator =  m_timer.begin();
	std::vector<TimerBase*>::const_iterator end = m_timer.end();

	for (timerIterator = timerIterator; timerIterator != end; timerIterator++)
	{
		TimerBase *const timer = (*timerIterator);
		if (timer != NULL)
		{
			timer->Destroy();
			delete timer;
		}
	}

	m_timer.clear();

	delete m_output;
	m_output = NULL;
}

/*
*	\brief	Equals operator
*/
void ProFy::operator=(
		ProFy const& other								//!< The other instance of the class
	)
{

}

/*
*	\brief	Get the instance of the ProFy singleton
*/
ProFy &ProFy::GetInstance()
{
	return instance;
}

/*
*	\brief	Create a timer of the given type, setting the timer id to the value of the newly created timers index
*	\return True on timer creation successful, false on failure to create the timer
*/
bool ProFy::CreateTimer(
		ProFy::TimerID &id,								//!< The variable to be set to the timer id
		ProFy::TimerType::Enum type,					//!< The type of timer to create
		std::string name,								//!< The name of the timer, used when outputting information to file
		const bool autoStart							//!< Start the timer if succesfully created
	)
{
	id = InvalidTimer;

	TimerBase *newTimer = NULL;

	switch (type)
	{
	case TimerType::CPU:
		{
			newTimer = new TimerCPU();
		}
		break;
	case TimerType::Direct3D:
		{

		}
		break;
	default:
		{
			// Invalid timer type specified
			return false;
		}
		break;
	}

	if (newTimer == NULL)
	{
		// failed to instantiate instance of timer class
		return false;
	}

	const bool createResult = newTimer->Create(name);
	if (createResult == false)
	{
		// Failed to create timer
		return false;
	}

	// Add the newly created timer to the timer list
	m_timer.push_back(newTimer);

	// Set the id of the timer, to the index of the new timer in the timer list
	id = m_timer.size();

	// If this timer should be auto started, start the timer
	if (autoStart == true)
	{
		newTimer->Start();
	}

	return true;
}

/*
*	\brief	Start a given timer
*/
void ProFy::StartTimer(
		TimerID &id										//!< The id of the timer to start
	)
{
    if (TimerExists(id)) {
        m_timer[id - 1]->Start();
    }
}

/*
*	\brief	End a given timer
*/
void ProFy::EndTimer(
		TimerID &id										//!< The id of the timer to end
	)
{
    if (TimerExists(id)) {
        m_timer[id - 1]->End();
    }
}

/*
*	\brief	Query a timer for its last known execution time
*	\return	The execution time in milliseconds
*/
float ProFy::GetTimerResult(
		TimerID &id										//!< The id of the timer to query
	)
{
	if (!TimerExists(id)) {
		return 0;
	}
	return m_timer[id - 1]->GetLastTime();
}

/*
*	\brief	Query a timer for its last known execution time
*	\return	The execution time in milliseconds
*/
std::vector<float> ProFy::GetTimerResults(
		TimerID &id,									//!< The id of the timer to query
		unsigned int noofResults
	)
{
	if (!TimerExists(id)) {
		std::vector<float> emptyList;
		return emptyList;
	}

	std::vector<float> results = m_timer[id - 1]->GetResults();
	std::vector<float> returnResults;

	unsigned int resultSize = results.size();
	if (resultSize == 0 || resultSize <= noofResults)
	{
		return results;
	}

	for (unsigned int resultIndex = resultSize - 1; resultIndex > (resultSize - noofResults - 1); --resultIndex)
	{
		returnResults.push_back(results[resultIndex]);
	}

	return returnResults;
}

/*
*	\brief
*/
void ProFy::AddFakeResult(
		ProFy::TimerID &id,							//!< The id of the timer to query
		float time
)
{
	m_timer[id - 1]->AddFakeResult(time);
}

/*
*	\brief	Query a timer for its last known execution time
*/
void ProFy::OutputAllTimers()
{
	std::vector<TimerBase*>::iterator timerIterator =  m_timer.begin();
	std::vector<TimerBase*>::const_iterator end = m_timer.end();

	unsigned int timerIndex = 1;
	for (timerIterator = timerIterator; timerIterator != end; timerIterator++)
	{
		OutputTimer(timerIndex);
		timerIndex++;
	}
}

/*
*	\brief	Query a timer for its last known execution time
*/
void ProFy::OutputTimer(
		TimerID &id,									//!< The id of the timer to output
		const bool openOutput							//!< Should the ouput file be opened by the default file program
	)
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

	TimerBase *const timer = m_timer[id - 1];
	std::stringstream filename;
	filename << timer->GetName() << "-" << date.str() << ".html";

	std::ofstream outputFile;
	outputFile.open(filename.str().c_str(), std::ios_base::app);

	m_output->Start(outputFile);
	m_output->AddLineGraph(outputFile, timer);
	m_output->End(outputFile);

	outputFile.close();

	if (openOutput) {
	    #ifdef _MSC_VER
            ShellExecute(0, 0, filename.str().c_str(), 0, 0 , SW_SHOWMAXIMIZED);
		#else

		#endif
	}
}

/*
*	\brief	Output a given list of timers
*/
void ProFy::OutputTimers(
		char *filename,
		std::vector<TimerID> &timerIDs,					//!< The list of timer ids to output
		GraphType::Enum graphType,						//!< The type of graph to output
		const bool openOutput							//!< Should the ouput file be opened by the default file program
	)
{
	std::vector<TimerBase*> timers;
	for (unsigned int idIndex = 0; idIndex < timerIDs.size(); ++idIndex)
	{
		if (!TimerExists(timerIDs[idIndex])) {
			continue;
		}

		timers.push_back(m_timer[timerIDs[idIndex] - 1]);
	}

	if (timers.empty()) return;

	time_t t = time(0);   // get time now
	struct tm now;
	#ifdef _MSC_VER
		localtime_s(&now, &t);
	#else
		now = *localtime(&t);
	#endif

	std::stringstream date;
	date << now.tm_mday << '-' << (now.tm_mon + 1) << '-' << (now.tm_year + 1900) << " " << now.tm_hour << "-" << now.tm_min << "-" << now.tm_sec;

	std::stringstream filenamepath;
	filenamepath << filename << "-" << date.str() << ".html";

	std::ofstream outputFile;
	outputFile.open(filenamepath.str().c_str(), std::ios_base::app);

	m_output->Start(outputFile);
	if (graphType == GraphType::Line)
	{
		m_output->AddLineGraphs(outputFile, timers);
	}
	else if (graphType == GraphType::Pie)
	{
		m_output->AddPieChart(outputFile, timers);
	}
	m_output->End(outputFile);

	outputFile.close();

	if (openOutput) {
	    #ifdef _MSC_VER
            ShellExecute(0, 0, filenamepath.str().c_str(), 0, 0 , SW_SHOWMAXIMIZED);
        #else

        #endif
	}
}

/*
*	\brief	Get a timers name
*/
const char* ProFy::GetTimerName(
		TimerID &id						//!< The id of the timer to query
	)
{
	if (!TimerExists(id)) {
		return NULL;
	}

	return m_timer[id - 1]->GetName().c_str();
}

bool ProFy::TimerExists(
		TimerID &id						//!< The id of the timer to validate
	)
{
	if (id <= 0) return false;
	if (id > m_timer.size()) return false;

	return m_timer[id - 1] != NULL;
}
