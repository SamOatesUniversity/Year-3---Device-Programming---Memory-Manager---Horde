#include "ProFySampler.h"
#include <imagehlp.h>

int SamplingThreadFunction(void *rawPtr)
{
	ProcessInfo *processInfo = static_cast<ProcessInfo*>(rawPtr);

	std::map<std::string, int> samples;
	int noofSamples = 0;

	while (processInfo->engine->IsRunning()) 
	{
		SDL_Delay(10);

		// hacky fall back, non thread safe sampling
		noofSamples++;
		const std::string currentMethod = processInfo->engine->GetCurrentMethod();
		if (!currentMethod.empty()) 
		{
			int noofPreviousSamples = samples[currentMethod];
			samples[currentMethod] = noofPreviousSamples + 1;
		}

		// proper sampling



		
	}

	// convert the samples to percentages and output to a pie chart
	float p = 100.0f / noofSamples;
	std::map<std::string, float> percentageSamples;
	
	std::map<std::string, int>::iterator it;
	for(it = samples.begin(); it != samples.end(); ++it)
	{
		percentageSamples[it->first] = it->second * p;
	}

	OutputToHtml(percentageSamples);

	// Free up any resources


	return 0;
}

void OutputToHtml( std::map<std::string, float> &samples )
{
	std::ofstream outputFile;
	outputFile.open("Sampling Results.html", std::ios_base::app);

	std::stringstream outputBuffer;
	outputBuffer << "<html>";
	outputBuffer << "<head>";
	outputBuffer << "<script type='text/javascript' src='https://www.google.com/jsapi'></script>";
	outputBuffer << "<script type='text/javascript'>";
	outputBuffer << "google.load('visualization', '1', {packages:['corechart']});";
	outputBuffer << "google.setOnLoadCallback(drawChart);";
	outputBuffer << "function drawChart() {";
	outputBuffer << "var data = google.visualization.arrayToDataTable([";
	outputBuffer << "['Function', 'Percentage Sampled'],";

	std::map<std::string, float>::iterator it;
	for(it = samples.begin(); it != samples.end(); ++it)
	{
		std::map<std::string, float>::iterator t = it;
		t++;
		if (t != samples.end())
		{
			// put a comma at the end
			outputBuffer << "['" << it->first << "',     " << it->second << "],";
		}
		else
		{
			// last element in the map
			outputBuffer << "['" << it->first << "',     " << it->second << "]";
		}
	}

	outputBuffer << "]);";
	outputBuffer << "var options = {";
	outputBuffer << "title: 'Sampling Results'";
	outputBuffer << "};";
	outputBuffer << "var chart = new google.visualization.PieChart(document.getElementById('chart_div'));";
	outputBuffer << "chart.draw(data, options);";
	outputBuffer << "}";
	outputBuffer << "</script>";
	outputBuffer << "</head>";
	outputBuffer << "<body>";
	outputBuffer << "<div id='chart_div' style='width: 100%; height: 100%;'></div>";
	outputBuffer << "</body>";
	outputBuffer << "</html>";

	outputFile << outputBuffer.str();
	outputFile.close();
}
