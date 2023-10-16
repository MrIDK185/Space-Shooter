#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Configuration.hpp"
#include "AnimatedSprite.hpp"

class Player : public AnimatedSprite
{
private:
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

public:
	//* static

	static std::shared_ptr<Player> NewPlayer(Configuration &config, SDL_Renderer *renderer, const char *path);

	//*non-static

	Player(SDL_Renderer *renderer, const char *path, float scale, float radius, const unsigned int frame_width,
		   const unsigned int frame_height, const unsigned int img_frames, const unsigned int img_types,
		   const unsigned int animations_per_second, float acceleration, float max_velocity, float friction,
		   unsigned int effect_duration_seconds, unsigned int rotation_speed);

	~Player() override;

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

	bool GetGemCollected() const;

	void SetGemCollected(bool new_status);

	Uint64 GetCollectionTicks() const;

	void SetCollectionTicks(Uint64 collection_ticks);

	void CheckType(Uint64 current_ticks);

	void HandleKeys(int screen_width, int screen_height, Uint64 delta_time_seconds, const Uint8 *keyboard);

	void Render() const override;
};

#endif //! PLAYER_HPP