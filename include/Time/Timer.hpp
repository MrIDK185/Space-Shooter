#ifndef SECOND_TIMER_HPP
#define SECOND_TIMER_HPP

#include <SDL2/SDL.h>

#include "Game/EventCode.hpp"
#include "Game/gameState.hpp"

struct Timer
{
private:
	SDL_TimerCallback const Callback;

	Uint32 timerID = 0;

public:
	bool Started = false;
	gameState *currentGameState;

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
		  gameState *game_state, void *custom_data = nullptr);

	~Timer();

	void operator=(const Timer &obj);

	void Start();

	void Pause();

	void Resume();

	void Stop();

	void Restart();
};

#endif //! SECOND_TIMER_HPP