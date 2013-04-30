#ifndef PROFY_HEADER_OUTPUTHTML
#define PROFY_HEADER_OUTPUTHTML

#include "../Timers/TimerBase.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <ctime>

class OutputHtml {

protected:

	friend class ProFy;

protected:

											//! Class Constructor
											OutputHtml();

											//! Class Copy Constructor
											OutputHtml(
												OutputHtml &instance					//!< Instance to instanciate from
											);

											//! Class Destructor
											~OutputHtml();

public:

											//! Start the output
	void									Start(
												std::ofstream &file						//!<
											);

											//! End the output
	void									End(
												std::ofstream &file						//!<
											);

											//! Add a line graph of a given timer
	void									AddLineGraph(
												std::ofstream &file,					//!<
												TimerBase *timer						//!< The timer to use
											);


											//! Add a line graph displaying multiple timers
	void									AddLineGraphs(
												std::ofstream &file,					//!<
												std::vector<TimerBase*> timers			//!< The timers to use
											);

											//! Add a pie chart displaying multiple timers
	void									AddPieChart(
												std::ofstream &file,					//!<
												std::vector<TimerBase*> timers			//!< The timers to use
											);

};

#endif
