#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "AnimatedSprite.hpp"

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

class Player : public AnimatedSprite
{
private:
	//* non-static

	float
		Acceleration,
		Velocity = 0,
		maxVelocity,
		Angle = 0,
		Friction;

	unsigned int
		effectDuration,
		rotationSpeed;

	Uint64 collectionTicks = 0;
	bool gemCollected = false;

	void UpdateAngle(float delta_time_seconds, const Uint8 *keyboard);

	void UpdateVelocity(float delta_time_seconds, const Uint8 *keyboard);

public:
	//* non-static

	Player(SpriteData sprite_data, AnimationData animation_data, PlayerData player_data);

	explicit Player(const Player &obj);

	explicit Player(Player &&obj);

	~Player() override = default;

	float GetAcceleration() const;

	void SetAcceleration(float acceleration);

	float GetVelocity() const;

	void SetVelocity(float velocity);

	float GetMaxVelocity() const;

	void SetMaxVelocity(float max_velocity);

	float GetAngle() const;

	void SetAngle(float angle);

	float GetFriction() const;

	void SetFriction(float friction);

	unsigned int GetEffectDuration() const;

	void SetEffectDuration(unsigned int effect_duration);

	unsigned int GetRotationSpeed() const;

	void SetRotationSpeed(unsigned int rotation_speed);

	bool GetGemCollected() const;

	void SetGemCollected(bool new_status);

	Uint64 GetCollectionTicks() const;

	void SetCollectionTicks(Uint64 current_ticks);

	void UpdateGemCollected(Uint64 current_ticks);

	void HandleInput(int screen_width, int screen_height, float delta_time_seconds, const Uint8 *keyboard);

	void Reset(float acceleration, float max_velocity, float friction);

	void Render() const override;
};

#endif //! PLAYER_HPP