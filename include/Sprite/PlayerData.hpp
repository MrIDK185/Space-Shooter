#ifndef PLAYER_DATA_HPP
#define PLAYER_DATA_HPP

typedef struct
{
	float
		Acceleration,
		maxVelocity,
		Friction;

	unsigned int
		effectDuration,
		rotationSpeed;
} PlayerData;

#endif //! PLAYER_DATA_HPP