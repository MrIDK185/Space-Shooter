#ifndef GEM_DATA_HPP
#define GEM_DATA_HPP

#include "Game/gameState.hpp"

typedef struct
{
	unsigned int
		blinkDuration,
		lifetimeDuration,
		minimumBrightness,
		maximumBrightness,
		blinkFactor;

	gameState *game_state;
} GemData;

#endif //! GEM_DATA_HPP