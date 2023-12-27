#ifndef SECOND_TIMER_HPP
#define SECOND_TIMER_HPP

#include <SDL2/SDL.h>

#include "Game/EventCode.hpp"

struct Timer
{
private:
	SDL_TimerCallback Callback = nullptr;

	Uint32 timerID = 0;

public:
	bool Started = false;

	EventCode tickEvent;
	EventCode endEvent;

	void *customData = nullptr;

	unsigned int
		durationMilliseconds = 0,
		intervalMilliseconds = 0,
		counterMilliseconds = 0;

	explicit Timer();

	Timer(EventCode tick_event, EventCode end_event,
		  unsigned int duration_milliseconds, unsigned int interval_milliseconds,
		  void *custom_data = nullptr);

	~Timer();

	void Start();

	void Stop();

	void Reset();
};

#endif //! SECOND_TIMER_HPP