#include "Sprite/Player.hpp"

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
		Angle -= 360;
	}
	else if (Angle < 0)
	{
		Angle += 360;
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

Player::Player(SpriteData sprite_data, AnimationData animation_data, PlayerData player_data)
	: AnimatedSprite(sprite_data, animation_data),
	  Acceleration(player_data.Acceleration),
	  maxVelocity(player_data.maxVelocity),
	  Friction(player_data.Friction),
	  effectDurationMilliseconds(player_data.effectDurationMilliseconds),
	  rotationSpeed(player_data.rotationSpeed)
{
	effectTimer = Timer(NO_EVENT, PLAYER_EFFECT_STOP, effectDurationMilliseconds, effectDurationMilliseconds,
						player_data.gameState, this);

	return;
}

Player::Player(const Player &obj)
	: AnimatedSprite(obj),
	  Acceleration(obj.Acceleration),
	  Velocity(obj.Velocity),
	  maxVelocity(obj.maxVelocity),
	  Angle(obj.Angle),
	  Friction(obj.Friction),
	  effectDurationMilliseconds(obj.effectDurationMilliseconds),
	  rotationSpeed(obj.rotationSpeed),
	  gemCollected(obj.gemCollected)
{
	effectTimer = Timer(NO_EVENT, PLAYER_EFFECT_STOP, effectDurationMilliseconds, effectDurationMilliseconds,
						obj.effectTimer.currentGameState, this);

	return;
}

Player::Player(Player &&obj)
	: AnimatedSprite(std::move(obj)),
	  Acceleration(obj.Acceleration),
	  Velocity(obj.Velocity),
	  maxVelocity(obj.maxVelocity),
	  Angle(obj.Angle),
	  Friction(obj.Friction),
	  effectDurationMilliseconds(obj.effectDurationMilliseconds),
	  rotationSpeed(obj.rotationSpeed),
	  gemCollected(obj.gemCollected)
{
	effectTimer = Timer(NO_EVENT, PLAYER_EFFECT_STOP, effectDurationMilliseconds, effectDurationMilliseconds,
						obj.effectTimer.currentGameState, this);

	return;
}

constexpr Player &Player::operator=(const Player &obj)
{
	Acceleration = obj.Acceleration;
	Velocity = obj.Velocity;
	maxVelocity = obj.maxVelocity;
	Angle = obj.Angle;
	Friction = obj.Friction;

	effectDurationMilliseconds = obj.effectDurationMilliseconds;
	rotationSpeed = obj.rotationSpeed;

	gemCollected = obj.gemCollected;
	// effectTimer = Timer(NO_EVENT, PLAYER_EFFECT_STOP, effectDurationMilliseconds, effectDurationMilliseconds,
	// obj.effectTimer.currentGameState, this);

	return *this;
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
	return effectDurationMilliseconds;
}

void Player::SetEffectDuration(unsigned int effect_duration_milliseconds)
{
	effectDurationMilliseconds = effect_duration_milliseconds;

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

void Player::StartEffect()
{
	gemCollected = true;
	effectTimer.Restart();

	return;
}

void Player::StopEffect()
{
	gemCollected = false;
	effectTimer.Stop();

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
	StopEffect();
	SetAngle(0);
	SetVelocity(0);
	SetAcceleration(acceleration);
	SetMaxVelocity(max_velocity);
	SetFriction(friction);

	return;
}

void Player::Render() const
{
	SDL_RenderCopyExF(*destRenderer, Texture.get(), &IMGPartRect, &Rect, Angle, nullptr, SDL_FLIP_NONE);

	return;
}