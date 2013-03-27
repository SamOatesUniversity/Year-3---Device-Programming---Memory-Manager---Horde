#include "lasagne-debug-stats.h"

CLasagneDebugStats::CLasagneDebugStats()
{

}

CLasagneDebugStats::~CLasagneDebugStats()
{

}

void CLasagneDebugStats::AddTimer( 
		ProFy::TimerID timer /*!< */ 
	)
{
	m_timers.push_back(timer);
}

void CLasagneDebugStats::Render(
		SDL_Surface *surface         //!< The surface to blit our text surface too
	)
{
	static const float timerHeight = 20.0f;
	int yPos = 40;

	for (unsigned int timerIndex = 0; timerIndex < m_timers.size(); ++timerIndex)
	{
		std::vector<unsigned int> results = ProFy::GetInstance().GetTimerResults(m_timers[timerIndex], 10);

		float maxTime = 0;
		float minTime = 0xFFFFFFFF;
		for (unsigned int resultIndex = 0; resultIndex < results.size(); ++resultIndex)
		{
			const float resultTime = static_cast<float>(results[resultIndex]);
			if (resultTime > maxTime) maxTime = resultTime;
			if (resultTime < minTime) minTime = resultTime;
		}

		const float scale = (timerHeight * 0.5f) / static_cast<float>(maxTime);

		int xPos = 4;
		const int xSpacing = 20;

		boxColor(surface, xPos, yPos, xPos + ((results.size() + 1) * xSpacing), static_cast<int>(yPos + timerHeight), 0x9400D377);
		rectangleColor(surface, xPos, yPos, xPos + ((results.size() + 1) * xSpacing), static_cast<int>(yPos + timerHeight), 0xFFFF00FF);

		const char* const name = ProFy::GetInstance().GetTimerName(m_timers[timerIndex]);
		stringColor(surface, xPos + 2, yPos + 2, name, 0xFFFFFFFF);

		xPos += xSpacing;
		const int bottomOfTimer = yPos + static_cast<unsigned int>(timerHeight);

		unsigned int lastTime = 0;
		for (unsigned int resultIndex = 0; resultIndex < results.size(); ++resultIndex)
		{
			const unsigned int resultTime = results[resultIndex];

			aalineColor(surface, xPos - xSpacing, static_cast<int>(bottomOfTimer - (lastTime * scale)), xPos, static_cast<int>(bottomOfTimer - (resultTime * scale)), 0xFFFFFFFF);

			xPos += xSpacing;
			lastTime = resultTime;
		}

		aalineColor(surface, xPos - xSpacing, static_cast<int>(bottomOfTimer - (lastTime * scale)), xPos, bottomOfTimer, 0xFFFFFFFF);

		xPos = 4 + xSpacing;
		for (unsigned int resultIndex = 0; resultIndex < results.size(); ++resultIndex)
		{
			const unsigned int resultTime = results[resultIndex];

			filledCircleColor(surface, xPos, static_cast<int>(bottomOfTimer - (resultTime * scale)), 2, 0xFF0000FF);

			xPos += xSpacing;
		}

		yPos += static_cast<int>(timerHeight + 6);		
	}
}
