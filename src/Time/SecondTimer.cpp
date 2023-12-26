#include "Time/SecondTimer.hpp"
#include "Game/Game.hpp"

static Uint32 TimerCallback(Uint32 interval_milliseconds, void *param)
{
	SecondTimer *timer = static_cast<SecondTimer *>(param);

	if (timer->currentGame->currentGameState == GAME_PAUSED)
	{
		return interval_milliseconds;
	}

	if (timer->counterMilliseconds >= interval_milliseconds)
	{
		timer->counterMilliseconds -= interval_milliseconds;
	}
	else
	{
		timer->counterMilliseconds = 0;
	}

	if (timer->counterMilliseconds > 0)
	{
		SDL_Event event;
		event.type = SDL_USEREVENT;
		event.user.code = TIMER_DECREMENT;
		SDL_PushEvent(&event);

		return interval_milliseconds;
	}

	SDL_Event event;
	event.type = SDL_USEREVENT;
	event.user.code = TIMER_STOP;
	SDL_PushEvent(&event);

	timer->Stop();

	return 0;
}

SecondTimer::SecondTimer()
	: Callback(TimerCallback)
{
}

SecondTimer::SecondTimer(unsigned int duration_milliseconds, unsigned int interval_milliseconds, Game *current_game)
	: Callback(TimerCallback),
	  durationMilliseconds(duration_milliseconds),
	  intervalMilliseconds(interval_milliseconds),
	  currentGame(current_game)
{
	counterMilliseconds = durationMilliseconds;

	return;
}

SecondTimer::~SecondTimer()
{
	Callback = nullptr;
	currentGame = nullptr;

	return;
}

void SecondTimer::Start()
{
	if (Started)
	{
		return;
	}

	timerID = SDL_AddTimer(intervalMilliseconds, Callback, this);
	if (timerID > 0)
	{
		Started = true;
	}

	return;
}

void SecondTimer::Stop()
{
	if (!Started)
	{
		return;
	}

	SDL_RemoveTimer(timerID);
	timerID = 0;
	Started = false;

	return;
}

void SecondTimer::Reset()
{
	timerID = 0;
	counterMilliseconds = durationMilliseconds;
	Started = false;

	return;
}