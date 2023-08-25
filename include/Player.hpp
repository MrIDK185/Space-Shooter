#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "AnimatedSprite.hpp"

class Player : public AnimatedSprite
{
private:
	//*static

	static SDL_Renderer *D_RENDERER;

	static float
		D_ACCELERATION,
		D_MAX_VELOCITY,
		D_SCALE, D_FRICTION,
		D_RADIUS;

	static unsigned int
		D_FRAME_WIDTH,
		D_FRAME_HEIGHT,
		D_IMG_FRAMES,
		D_IMG_TYPES,
		D_ANIMATIONS_PER_SECOND,
		D_EFFECT_DURATION_SECONDS,
		D_ROTATION_SPEED;

	//*non-static

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

	Configuration &Config;

public:
	//*static

	static void InitDefaultValues(SDL_Renderer *renderer, float scale, float radius, const unsigned int frame_width, const unsigned int frame_height, const unsigned int img_frames, const unsigned int img_types, const unsigned int animations_per_second, float acceleration, float max_velocity, float friction, unsigned int effect_duration_seconds, unsigned int rotation_speed);

	static void CleanDefaultValues();

	//*non-static

	Player(const char *path, Configuration &config);

	~Player() override;

	float GetAcceleration();

	void SetAcceleration(float acceleration);

	float GetVelocity();

	void SetVelocity(float velocity);

	float GetMaxVelocity();

	void SetMaxVelocity(float max_velocity);

	float GetAngle();

	void SetAngle(float angle);

	float GetFriction();

	void SetFriction(float friction);

	bool GetGemCollected();

	void SetGemCollected(bool new_status);

	Uint64 GetCollectionTicks();

	void SetCollectionTicks(Uint64 collection_ticks);

	void CheckType(Uint64 current_ticks);

	void HandleKeys();

	void Render() override;
};

#endif //! PLAYER_HPP