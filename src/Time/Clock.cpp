#include "Time/Clock.hpp"

//*non-static(public)

void Clock::Tick()
{
	LAST = NOW;
	NOW = SDL_GetPerformanceCounter();
	const Uint64 delta_ticks = NOW - LAST;
	const Uint64 TICKS_PER_SECOND = SDL_GetPerformanceFrequency();

	deltaTimeSeconds = static_cast<float>(delta_ticks) / static_cast<float>(TICKS_PER_SECOND);
	framesPerSecond = 1.0f / deltaTimeSeconds;

	return;
}