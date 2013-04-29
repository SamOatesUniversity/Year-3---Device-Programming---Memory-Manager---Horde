#include "OutputHtml.h"

/*
*	\brief	Class constructor
*/
OutputHtml::OutputHtml()
{

}

/*
*	\brief	Class copy constructor
*/
OutputHtml::OutputHtml(
		OutputHtml &instance							//!< Instance to instanciate from
	)
{

}

/*
*	\brief	Class destructor
*/
OutputHtml::~OutputHtml()
{

}

/*
*	\brief	Start the output
*/
void OutputHtml::Start(
		std::ofstream &file								//!<
	)
{
	std::stringstream outputBuffer;
	outputBuffer << "<html>\n";
	outputBuffer << "<head>\n";
	outputBuffer << "<script type=\"text/javascript\" src=\"https://www.google.com/jsapi\"></script>\n";
	outputBuffer << "<script type=\"text/javascript\">\n";
	outputBuffer << "google.load(\"visualization\", \"1\", {packages:[\"corechart\", \"table\"]});\n";
	outputBuffer << "google.setOnLoadCallback(drawChart);\n";

	file << outputBuffer.str();
}

/*
*	\brief	End the output
*/
void OutputHtml::End(
		std::ofstream &file								//!<
	)
{
	std::stringstream outputBuffer;
	outputBuffer << "</script>\n";
	outputBuffer << "</head>\n";
	outputBuffer << "<body>\n";

	outputBuffer << "<table width=\"100%\" height=\"100%\">\n";
	outputBuffer << "<tr><td align=\"center\" valign=\"middle\">\n";
	outputBuffer << "<div id=\"table_div\" style=\"\"></div>\n";
	outputBuffer << "</td><td align=\"center\" valign=\"middle\">\n";
	outputBuffer << "<div id=\"chart_div\" style=\"width: 100%; height: 100%;\"></div>\n";
	outputBuffer << "</td></tr>\n";
	outputBuffer << "</table>\n";

	outputBuffer << "</body>\n";
	outputBuffer << "</html>\n";

	file << outputBuffer.str();
}

/*
*	\brief	Add a line graph of a given timer
*/
void OutputHtml::AddLineGraph(
		std::ofstream &file,							//!<
		TimerBase *timer								//!< The timer to use
	)
{
	std::stringstream outputBuffer;
	outputBuffer << "function drawChart() {\n";
	outputBuffer << "var data = google.visualization.arrayToDataTable([\n";
	outputBuffer << "['Result Index', '" << timer->GetName() << "'],\n";

	std::vector<float> results = timer->GetResults();
	if (results.size() > 0)
	{
		for (unsigned int resultIndex = 0; resultIndex < results.size() - 1; ++resultIndex)
		{
			float result = results[resultIndex];
			outputBuffer << "['" << resultIndex << "', " << result << "],\n";
		}
		outputBuffer << "['" << results.size() - 1 << "', " << results[results.size() - 1] << "]\n";
	}

	outputBuffer << "]);\n";
	outputBuffer << "var options = {\n";
	outputBuffer << "};\n";
	outputBuffer << "var chart = new google.visualization.LineChart(document.getElementById('chart_div'));\n";
	outputBuffer << "chart.draw(data, options);\n";
	outputBuffer << "}\n";

	file << outputBuffer.str();
}

/*
*	\brief	Add a line graph displaying multiple timers
*/
void OutputHtml::AddLineGraphs(
		std::ofstream &file,							//!<
		std::vector<TimerBase*> timers					//!< The timer to use
	)
{
	unsigned int biggestNoofResults = 0;
	for (unsigned int timerIndex = 0; timerIndex < timers.size(); ++timerIndex)
	{
		TimerBase *const timer = timers[timerIndex];
		unsigned int noofTimerResults = timer->GetResults().size();
		if (biggestNoofResults < noofTimerResults) {
			biggestNoofResults = noofTimerResults;
		}
	}

	std::stringstream outputBuffer;
	outputBuffer << "function drawChart() {\n";
	outputBuffer << "var data = google.visualization.arrayToDataTable([\n";
	outputBuffer << "['Result Index', ";

	for (unsigned int timerIndex = 0; timerIndex < timers.size() - 1; ++timerIndex)
	{
		TimerBase *const timer = timers[timerIndex];
		outputBuffer << "'" <<timer->GetName() << "', ";
	}
	outputBuffer << "'" << timers[timers.size() - 1]->GetName() << "'],\n";

	for (unsigned int resultIndex = 0; resultIndex < biggestNoofResults; ++resultIndex)
	{
		outputBuffer << "['" << resultIndex << "', ";

		for (unsigned int timerIndex = 0; timerIndex < timers.size(); ++timerIndex)
		{
			TimerBase *const timer = timers[timerIndex];
			std::vector<float> results = timer->GetResults();
			float result = 0;
			if (results.size() > resultIndex) {
				result = results[resultIndex];
			}

			outputBuffer << result;
			if (timerIndex != timers.size() - 1)
				outputBuffer << ", ";
		}

		outputBuffer << "]";

		if (resultIndex != biggestNoofResults - 1)
			outputBuffer << ",";

		outputBuffer << "\n";
	}

	outputBuffer << "]);\n";
	outputBuffer << "var options = {\n";
	outputBuffer << "hAxis: {title: 'Result Index'},";
	outputBuffer << "vAxis: {title: 'Time (ms)'}";
	outputBuffer << "};\n";
	outputBuffer << "var chart = new google.visualization.LineChart(document.getElementById('chart_div'));\n";
	outputBuffer << "chart.draw(data, options);\n";
	outputBuffer << "}\n";

	file << outputBuffer.str();
}

void OutputHtml::AddPieChart(
		std::ofstream &file,				/*!< */
		std::vector<TimerBase*> timers		/*!< The timers to use */
	)
{
	std::stringstream outputBuffer;
	outputBuffer << "function drawChart() {\n";
	outputBuffer << "var data = google.visualization.arrayToDataTable([\n";
	outputBuffer << "['Timer', 'Average Time (ms)'],\n";

	int noofTimers = timers.size();
	for (int timerIndex = 0; timerIndex < noofTimers; ++timerIndex)
	{
	    TimerBase *timer = timers[timerIndex];
		noofTimers--;
		const float averageTime = timer->CalculateAverageTime();
		outputBuffer << "['" << timer->GetName() << "', " << averageTime << "]";

		if (noofTimers != 0)
		{
			outputBuffer << ",";
		}
		outputBuffer << "\n";
	}

	outputBuffer << "]);\n";
	outputBuffer << "var options = {\n";
	outputBuffer << "};\n";
	outputBuffer << "var chart = new google.visualization.PieChart(document.getElementById('chart_div'));\n";
	outputBuffer << "chart.draw(data, options);\n";

	outputBuffer << "var table = new google.visualization.Table(document.getElementById('table_div'));\n";
	outputBuffer << "table.draw(data, {showRowNumber: false});\n";

	outputBuffer << "}\n";


	file << outputBuffer.str();
}
