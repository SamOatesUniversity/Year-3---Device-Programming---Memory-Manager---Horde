
#ifndef HEADER_PROFY
#define HEADER_PROFY

#include "Timers/Timers.h"
#include "Output/OutputHtml.h"

#include <vector>

#define										InvalidTimer	0								//!< Represents the value of an invalid timer

struct GraphType {
	enum Enum {
		Line,
		Pie,
		Noof
	};
};

class ProFy {

private:

	static ProFy							instance;
	std::vector<TimerBase*>					m_timer;										//!< List of currently active timers
	OutputHtml								*m_output;										//!< Class used to output information

private:
											//! Class constructor
											ProFy();

											//! Class copy constructor
											ProFy(
												ProFy const& instance						//!< The instance to instanciate from
											);

											//! Class Destructor
											~ProFy();

											//! Equals operator
	void									operator=(
												ProFy const& other							//!< The other instance of the class
											);

public:

	typedef	unsigned int					TimerID;										//!< Type def for the timer id used in creating and accesing timers

	struct TimerType {
		enum Enum {
											CPU,											//!< Cpu timer
											OpenGL,											//!< OpenGL timer
											Direct3D,										//!< Direct3D timer
											Noof											//!< Number of timer types
		};
	};

public:

											//! Get the instance of the ProFy singleton
	static ProFy							&GetInstance();

											//! Create a timer of the given type, setting the timer id and returning true on success, else return false on failure
	bool									CreateTimer(
												ProFy::TimerID &id,							//!< The variable to be set to the timer id
												ProFy::TimerType::Enum type,				//!< The type of timer to create
												std::string name,							//!< The name of the timer, used when outputting information to file
												const bool autoStart = false				//!< Start the timer if succesfully created
											);

											//! Start a given timer
	void									StartTimer(
												ProFy::TimerID &id							//!< The id of the timer to start
											);

											//! End a given timer
	void									EndTimer(
												ProFy::TimerID &id							//!< The id of the timer to end
											);

											//! Get a timers name
	const char*								GetTimerName(
												ProFy::TimerID &id							//!< The id of the timer to query
											);

											//! Query a timer for its last known execution time
	float									GetTimerResult(
												ProFy::TimerID &id							//!< The id of the timer to query
											);

											//! Query a timer for a given number of results
	std::vector<float>						GetTimerResults(
												ProFy::TimerID &id,							//!< The id of the timer to query
												unsigned int noofResults					//!< The number of results to get
											);

											//! Add a result to the timer (used for adding comparison data, e.g. if something should tak x time)
	void									AddFakeResult(
												ProFy::TimerID &id,							//!< The id of the timer to effect
												float time									//!< The result to add
											);

											//! Output all the current timers
	void									OutputAllTimers();

											//! Output a given timer
	void									OutputTimer(
												ProFy::TimerID &id,							//!< The id of the timer to output
												const bool openOutput = false				//!< Should the ouput file be opened by the default file program
											);

											//! Output a given list of timers
	void									OutputTimers(
												char *filename,								//!< Propossed file name of the graph
												std::vector<ProFy::TimerID> &timerIDs,		//!< The list of timer ids to output
												GraphType::Enum graphType,					//!< The type of graph to output
												const bool openOutput = false				//!< Should the ouput file be opened by the default file program
											);

											//! Check if a timer by the given id exists
	bool									TimerExists(
												ProFy::TimerID &id							//!< The id of the timer to validate
											);
};

#endif
