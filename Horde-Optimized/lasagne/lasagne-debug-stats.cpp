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
	static const float timerHeight = 40.0f;
	int yPos = 40;

	for (unsigned int timerIndex = 0; timerIndex < m_timers.size(); ++timerIndex)
	{
		std::vector<unsigned int> results = ProFy::GetInstance().GetTimerResults(m_timers[timerIndex], 10);
		if (results.empty()) {
			continue;
		}

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
		const int xSpacing = 12;

		int boxWidth = xPos + ((results.size() + 1) * xSpacing);
		boxColor(surface, xPos, yPos, boxWidth, static_cast<int>(yPos + timerHeight), 0x9400D377);
		rectangleColor(surface, xPos, yPos, boxWidth, static_cast<int>(yPos + timerHeight), 0xFFFF00FF);

		const char* const name = ProFy::GetInstance().GetTimerName(m_timers[timerIndex]);
		stringColor(surface, xPos + 2, yPos + 2, name, 0xFFFFFFFF);

		std::stringstream maxStr, minStr;
		maxStr << maxTime << "ms"; minStr << minTime << "ms";
		stringColor(surface, boxWidth + 2, yPos, maxStr.str().c_str(), 0xFFFFFFFF);
		stringColor(surface, boxWidth + 2, static_cast<int>(yPos + timerHeight) - 6, minStr.str().c_str(), 0xFFFFFFFF);

		xPos += xSpacing;
		const int bottomOfTimer = yPos + static_cast<unsigned int>(timerHeight);

		unsigned int lastTime = 0;
		for (unsigned int resultIndex = 0; resultIndex < results.size(); ++resultIndex)
		{
			const unsigned int resultTime = results[resultIndex];

			if (resultIndex == 0) {
				lastTime = resultTime;
			}

			aalineColor(surface, xPos - xSpacing, static_cast<int>(bottomOfTimer - (lastTime * scale)), xPos, static_cast<int>(bottomOfTimer - (resultTime * scale)), 0xFFFFFFFF);

			xPos += xSpacing;
			lastTime = resultTime;
		}

		aalineColor(surface, xPos - xSpacing, static_cast<int>(bottomOfTimer - (lastTime * scale)), xPos, static_cast<int>(bottomOfTimer - (lastTime * scale)), 0xFFFFFFFF);

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
