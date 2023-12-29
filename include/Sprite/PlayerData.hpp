#ifndef PLAYER_DATA_HPP
#define PLAYER_DATA_HPP

#include "Game/gameState.hpp"

typedef struct
{
	unsigned int
		Acceleration,
		maxVelocity,
		Friction,
		effectDurationMilliseconds,
		rotationSpeed;

	gameState *gameState;
} PlayerData;

#endif //! PLAYER_DATA_HPP