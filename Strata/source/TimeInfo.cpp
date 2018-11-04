#include <switch.h>
#include "TimeInfo.h"

namespace Strata
{
	TimeInfo::TimeInfo()
	{
		// Initialize our variables
		frameCount = 0ULL;
		secondsPerTick = 0.0f;
		applicationTime = 0.0f;
		elapsedTime = 0.0f;
		rawApplicationTime = 0ULL;
		rawTime = 0ULL;
		timeScale = 1.0f;
		lastFPSTime = 0.0f;
		frameRate = 0.0f;
	}

	void TimeInfo::Initialize(void)
	{
		// Get the frequency of the timer
		u64 qwTicksPerSec;
		qwTicksPerSec = armGetSystemTickFreq();
		secondsPerTick = (float)(1.0f / qwTicksPerSec);

		// Save the start time
		rawTime = armGetSystemTick();

		// Zero out hte elapsed and total time variables
		rawApplicationTime = 0ULL;
		applicationTime = 0.0f;
		elapsedTime = 0.0f;
		frameCount = 0ULL;
	}

	void TimeInfo::UpdateTime(void)
	{
		u64 qwNewTime;
		u64 qwDeltaTime;

		qwNewTime = armGetSystemTick();
		qwDeltaTime = qwNewTime - rawTime;

		rawApplicationTime += qwDeltaTime;
		rawTime = qwNewTime;

		elapsedTime = secondsPerTick * ((float)(qwDeltaTime));
		applicationTime = secondsPerTick * ((float)(rawApplicationTime));

		frameCount++;
	}

	void TimeInfo::EndFrame(void)
	{
		if (applicationTime - lastFPSTime > 1.0f)
		{
			frameRate = frameCount / (applicationTime - lastFPSTime);
			frameCount = 0ULL;
			lastFPSTime = applicationTime;
		}
	}
}