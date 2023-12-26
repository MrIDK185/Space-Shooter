#include "Time/Clock.hpp"

void Clock::Tick()
{
	Last = Now;
	Now = SDL_GetPerformanceCounter();
	const float TICKS_PER_FRAME = static_cast<float>(Now - Last);
	const float TICKS_PER_SECOND = static_cast<float>(SDL_GetPerformanceFrequency());

	deltaTimeSeconds = TICKS_PER_FRAME / TICKS_PER_SECOND;
	framesPerSecond = TICKS_PER_SECOND / TICKS_PER_FRAME;

	return;
}