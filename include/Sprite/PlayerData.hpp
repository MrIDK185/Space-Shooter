#ifndef PLAYER_DATA_HPP
#define PLAYER_DATA_HPP

#include "Game/gameState.hpp"

typedef struct
{
	float
		Acceleration,
		maxVelocity,
		Friction;

	unsigned int
		effectDuration,
		rotationSpeed;

	gameState *gameState;
} PlayerData;

#endif //! PLAYER_DATA_HPP