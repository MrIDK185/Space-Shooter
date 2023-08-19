#include "Time.hpp"

#include <SDL2/SDL.h>

/*------------ Clock ------------*/

//*non-static(public)

void Clock::Tick()
{
	LAST = NOW;
	NOW = SDL_GetPerformanceCounter();
	const Uint64 delta = NOW - LAST;
	const Uint64 TICKS_PER_SECOND = SDL_GetPerformanceFrequency();
	deltaTimeSeconds = static_cast<float>(delta) / static_cast<float>(TICKS_PER_SECOND);

	return;
}

/*------------ SecondTimer ------------*/

//*non-static(public)

void SecondTimer::Start(Uint64 current_ticks)
{
	nextTicks = current_ticks + 1000 * Interval;
	Started = true;

	return;
}

void SecondTimer::Stop()
{
	Started = false;

	return;
}

bool SecondTimer::Check(Uint64 current_ticks)
{
	if (Started && current_ticks >= nextTicks)
	{
		nextTicks = current_ticks + 1000 * Interval;
		return true;
	}

	return false;
}