#ifndef GEM_DATA_HPP
#define GEM_DATA_HPP

#include "Game/gameState.hpp"

struct GemData
{
	unsigned int
		blinkDurationMilliseconds,
		lifetimeDurationMilliseconds,
		minimumBrightness,
		maximumBrightness,
		blinkFactor;

	gameState *game_state;
};

#endif //! GEM_DATA_HPP