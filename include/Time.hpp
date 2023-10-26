#ifndef TIME_HPP
#define TIME_HPP

#include <SDL2/SDL.h>

/*------------ Clock ------------*/

struct Clock
{
public:
	//*non-static

	Uint64
		NOW = 0,
		LAST = 0;

	float deltaTimeSeconds = 0;

	unsigned int framesPerSecond = 0;

	void Tick();

	Clock() = default;
	~Clock() = default;
};

/*------------ SecondTimer ------------*/

typedef enum
{
	TIMER_DECREMENT,
	TIMER_STOP
} timerEvent;

Uint32 TimerCallback(Uint32 interval_milliseconds, void *param);

struct SecondTimer
{
private:
	SDL_TimerCallback Callback = nullptr;

public:
	//*non-static

	unsigned int
		durationMilliseconds = 0,
		intervalMilliseconds = 0,
		counterMilliseconds = 0;

	Uint32 timerID = 0;

	bool Started = false;

	explicit SecondTimer();
	SecondTimer(unsigned int duration_milliseconds, unsigned int interval_milliseconds, SDL_TimerCallback callback);
	~SecondTimer();

	void Start();

	void Stop();

	void Reset();
};

#endif //! TIME_HPP
