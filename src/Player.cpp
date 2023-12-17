#include "Player.hpp"

//* static(public)

std::unique_ptr<Player> Player::NewPlayer(Configuration &config, SDL_Renderer *renderer, std::string path)
{
	return std::make_unique<Player>(renderer, path, config.PLAYER_SCALE, config.PLAYER_RADIUS, config.PLAYER_FRAME_WIDTH,
									config.PLAYER_FRAME_HEIGHT, config.PLAYER_IMG_FRAMES, config.PLAYER_IMG_TYPES,
									config.PLAYER_ANIMATIONS_PER_SECOND, config.PLAYER_ACCELEARION, config.PLAYER_MAX_VELOCITY,
									config.PLAYER_FRICTION, config.PLAYER_EFFECT_DURATION_SECONDS, config.PLAYER_ROTATION_SPEED);
}

//* non-static(private)

void Player::UpdateAngle(float delta_time_seconds, const Uint8 *keyboard)
{
	if (keyboard[SDL_SCANCODE_LEFT])
	{
		Angle -= rotationSpeed * delta_time_seconds;
	}
	if (keyboard[SDL_SCANCODE_RIGHT])
	{
		Angle += rotationSpeed * delta_time_seconds;
	}

	if (Angle >= 360)
	{
		Angle = 0;
	}
	else if (Angle < 0)
	{
		Angle = 360 + Angle;
	}

	return;
}

void Player::UpdateVelocity(float delta_time_seconds, const Uint8 *keyboard)
{
	if (keyboard[SDL_SCANCODE_UP])
	{
		Velocity += Acceleration * delta_time_seconds;
		gemCollected == false ? SetAnimationType(3) : SetAnimationType(4);
	}
	else
	{
		Velocity -= Friction * delta_time_seconds;
		gemCollected == false ? SetAnimationType(1) : SetAnimationType(2);
	}
	if (keyboard[SDL_SCANCODE_DOWN])
	{
		Velocity -= Acceleration * delta_time_seconds;
	}

	if (Velocity < 0)
	{
		Velocity = 0;
	}
	else if (Velocity > maxVelocity)
	{
		Velocity = maxVelocity;
	}

	return;
}

//* non-static(public)

Player::Player(SDL_Renderer *renderer, std::string path, float scale, float radius, const unsigned int frame_width,
			   const unsigned int frame_height, const unsigned int img_frames, const unsigned int img_types,
			   const unsigned int animations_per_second, float acceleration, float max_velocity, float friction,
			   unsigned int effect_duration_seconds, unsigned int rotation_speed)
	: AnimatedSprite(renderer, path, scale, radius, frame_width, frame_height, img_frames, img_types,
					 animations_per_second),
	  Acceleration(acceleration),
	  maxVelocity(max_velocity),
	  Friction(friction),
	  effectDuration(effect_duration_seconds * 1000),
	  rotationSpeed(rotation_speed)
{
}

float Player::GetAcceleration() const
{
	return Acceleration;
}

void Player::SetAcceleration(float acceleration)
{
	Acceleration = acceleration;

	return;
}

float Player::GetVelocity() const
{
	return Velocity;
}

void Player::SetVelocity(float velocity)
{
	Velocity = velocity;

	return;
}

float Player::GetMaxVelocity() const
{
	return maxVelocity;
}

void Player::SetMaxVelocity(float max_velocity)
{
	maxVelocity = max_velocity;

	return;
}

float Player::GetAngle() const
{
	return Angle;
}

void Player::SetAngle(float angle)
{
	Angle = angle;

	return;
}

float Player::GetFriction() const
{
	return Friction;
}

void Player::SetFriction(float friction)
{
	Friction = friction;

	return;
}

unsigned int Player::GetEffectDuration() const
{
	return effectDuration;
}

void Player::SetEffectDuration(unsigned int effect_duration)
{
	effectDuration = effect_duration;

	return;
}

unsigned int Player::GetRotationSpeed() const
{
	return rotationSpeed;
}

void Player::SetRotationSpeed(unsigned int rotation_speed)
{
	rotationSpeed = rotation_speed;

	return;
}

bool Player::GetGemCollected() const
{
	return gemCollected;
}

void Player::SetGemCollected(bool new_status)
{
	gemCollected = new_status;

	return;
}

Uint64 Player::GetCollectionTicks() const
{
	return collectionTicks;
}

void Player::SetCollectionTicks(Uint64 current_ticks)
{
	collectionTicks = current_ticks + effectDuration;

	return;
}

void Player::UpdateGemCollected(Uint64 current_ticks)
{
	if (gemCollected && current_ticks >= collectionTicks)
	{
		gemCollected = false;
	}

	return;
}

void Player::HandleInput(int screen_width, int screen_height, float delta_time_seconds, const Uint8 *keyboard)
{
	UpdateAngle(delta_time_seconds, keyboard);
	UpdateVelocity(delta_time_seconds, keyboard);

	Rect.x += std::sin(Angle * M_PI / 180) * Velocity * delta_time_seconds;
	Rect.y -= std::cos(Angle * M_PI / 180) * Velocity * delta_time_seconds;

	if (Rect.x <= -Rect.w)
	{
		Rect.x = screen_width;
	}
	else if (Rect.x >= screen_width)
	{
		Rect.x = -Rect.w;
	}
	if (Rect.y <= -Rect.h)
	{
		Rect.y = screen_height;
	}
	else if (Rect.y >= screen_height)
	{
		Rect.y = -Rect.h;
	}

	return;
}

void Player::Reset(float acceleration, float max_velocity, float friction)
{
	SetGemCollected(false);
	SetCollectionTicks(0);
	SetAngle(0);
	SetVelocity(0);
	SetAcceleration(acceleration);
	SetMaxVelocity(max_velocity);
	SetFriction(friction);

	return;
}

void Player::Render() const
{
	SDL_RenderCopyExF(destRenderer, Texture, &IMGPartRect, &Rect, Angle, nullptr, SDL_FLIP_NONE);

	return;
}