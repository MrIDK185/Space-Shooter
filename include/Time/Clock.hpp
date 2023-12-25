#ifndef CLOCK_HPP
#define CLOCK_HPP

#include <SDL2/SDL.h>

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

#endif //! CLOCK_HPP