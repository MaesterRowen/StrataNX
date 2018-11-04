#pragma once
#include <switch.h>


namespace Strata
{
	class TimeInfo
	{
	private:
		// Private variables
		float applicationTime;
		float elapsedTime;
		float timeScale;
		float secondsPerTick;
		float lastFPSTime;
		float frameRate;
		u64 rawTime;
		u64 rawApplicationTime;
		u64 frameCount;

	public:
		// Constructor
		TimeInfo();

		// Functional methods
		void Initialize(void);
		void UpdateTime(void);

		// Accessor methods
		float GetFrameRate() { return frameRate; }
		void EndFrame();
		float GetApplicationTime() { return applicationTime; }
		float GetElapsedTime() { return elapsedTime; }
		float GetTimeScale() { return timeScale; }
		void SetTimeScale(float scale) { timeScale = scale; }
	};
}