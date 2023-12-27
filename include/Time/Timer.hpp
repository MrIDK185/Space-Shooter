#ifndef SECOND_TIMER_HPP
#define SECOND_TIMER_HPP

#include <SDL2/SDL.h>

#include "Time/timerEvent.hpp"
#include "Game/customEvent.hpp"

struct Timer
{
private:
	SDL_TimerCallback Callback = nullptr;

	Uint32 timerID = 0;

public:
	bool Started = false;

	CustomEvent tickEvent;
	CustomEvent endEvent;

	void *customData = nullptr;

	unsigned int
		durationMilliseconds = 0,
		intervalMilliseconds = 0,
		counterMilliseconds = 0;

	explicit Timer();

	Timer(CustomEvent tick_event, CustomEvent end_event,
		  unsigned int duration_milliseconds, unsigned int interval_milliseconds,
		  void *custom_data = nullptr);

	~Timer();

	void Start();

	void Stop();

	void Reset();
};

#endif //! SECOND_TIMER_HPP