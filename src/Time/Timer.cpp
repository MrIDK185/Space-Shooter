#include "Time/Timer.hpp"
#include "Game/EventType.hpp"

static Uint32 TimerCallback(Uint32 interval_milliseconds, void *param)
{
	Timer *timer = static_cast<Timer *>(param);

	if (*timer->currentGameState == GAME_PAUSED)
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
		event.user.type = USEREVENT_TIMER;
		event.user.code = timer->tickEvent;
		event.user.data1 = timer;
		event.user.data2 = timer->customData;
		SDL_PushEvent(&event);

		return interval_milliseconds;
	}

	SDL_Event event;
	event.type = SDL_USEREVENT;
	event.user.type = USEREVENT_TIMER;
	event.user.code = timer->endEvent;
	event.user.data1 = timer;
	event.user.data2 = timer->customData;
	SDL_PushEvent(&event);

	return 0;
}

Timer::Timer()
	: Callback(TimerCallback)
{
}

Timer::Timer(EventCode tick_event, EventCode end_event,
			 unsigned int duration_milliseconds, unsigned int interval_milliseconds,
			 gameState *game_state, void *custom_data /*nullptr*/)
	: Callback(TimerCallback),
	  currentGameState(game_state),
	  tickEvent(tick_event),
	  endEvent(end_event),
	  customData(custom_data),
	  durationMilliseconds(duration_milliseconds),
	  intervalMilliseconds(interval_milliseconds),
	  counterMilliseconds(duration_milliseconds)
{
}

Timer::~Timer()
{
	Callback = nullptr;

	return;
}

void Timer::Start()
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

void Timer::Pause()
{
	if (!Started)
	{
		return;
	}

	SDL_RemoveTimer(timerID);
	timerID = 0;
	Started = false;
}

void Timer::Resume()
{
	Start();

	return;
}

void Timer::Stop()
{
	if (!Started)
	{
		return;
	}

	SDL_RemoveTimer(timerID);
	timerID = 0;
	Started = false;
	counterMilliseconds = durationMilliseconds;

	return;
}

void Timer::Restart()
{
	Stop();
	Start();

	return;
}