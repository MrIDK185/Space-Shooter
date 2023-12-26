#ifndef SECOND_TIMER_HPP
#define SECOND_TIMER_HPP

#include <SDL2/SDL.h>

#include "Time/timerEvent.hpp"

struct Game;

struct SecondTimer
{
private:
	SDL_TimerCallback Callback = nullptr;

public:
	unsigned int
		durationMilliseconds = 0,
		intervalMilliseconds = 0,
		counterMilliseconds = 0;

	Game *currentGame;

	Uint32 timerID = 0;

	bool Started = false;

	explicit SecondTimer();

	SecondTimer(unsigned int duration_milliseconds, unsigned int interval_milliseconds, Game *current_game);

	~SecondTimer();

	void Start();

	void Stop();

	void Reset();
};

#endif //! SECOND_TIMER_HPP