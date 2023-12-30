#ifndef PLAYER_DATA_HPP
#define PLAYER_DATA_HPP

#include "Game/gameState.hpp"

struct PlayerData
{
	unsigned int
		Acceleration,
		maxVelocity,
		Friction,
		effectDurationMilliseconds,
		rotationSpeed;

	gameState *gameState;
};

#endif //! PLAYER_DATA_HPP