#ifndef CLOCK_HPP
#define CLOCK_HPP

#include <SDL2/SDL.h>

struct Clock
{
public:
	Uint64
		Now = 0,
		Last = 0;

	float deltaTimeSeconds = 0,
		  framesPerSecond = 0;

	void Tick();

	Clock() = default;
	~Clock() = default;
};

#endif //! CLOCK_HPP