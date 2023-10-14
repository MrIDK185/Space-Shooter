#include "Player.hpp"

//*static(private)

SDL_Renderer *Player::D_RENDERER = nullptr;

float
	Player::D_ACCELERATION = 0,
	Player::D_MAX_VELOCITY = 0,
	Player::D_SCALE = 0,
	Player::D_FRICTION = 0,
	Player::D_RADIUS = 0;

unsigned int
	Player::D_FRAME_WIDTH = 0,
	Player::D_FRAME_HEIGHT = 0,
	Player::D_IMG_FRAMES = 0,
	Player::D_IMG_TYPES = 0,
	Player::D_ANIMATIONS_PER_SECOND = 0,
	Player::D_EFFECT_DURATION_SECONDS = 0,
	Player::D_ROTATION_SPEED = 0;

//*static(public)

void Player::InitDefaultValues(SDL_Renderer *renderer, float scale, float radius, const unsigned int frame_width, const unsigned int frame_height, const unsigned int img_frames, const unsigned int img_types, const unsigned int animations_per_second, float acceleration, float max_velocity, float friction, unsigned int effect_duration_seconds, unsigned int rotation_speed)
{
	D_RENDERER = renderer;
	D_SCALE = scale;
	D_RADIUS = radius;
	D_FRAME_WIDTH = frame_width;
	D_FRAME_HEIGHT = frame_height;
	D_IMG_FRAMES = img_frames;
	D_IMG_TYPES = img_types;
	D_ANIMATIONS_PER_SECOND = animations_per_second;
	D_ACCELERATION = acceleration;
	D_MAX_VELOCITY = max_velocity;
	D_FRICTION = friction;
	D_EFFECT_DURATION_SECONDS = effect_duration_seconds;
	D_ROTATION_SPEED = rotation_speed;

	return;
}

void Player::CleanDefaultValues()
{
	D_RENDERER = nullptr;

	return;
}

//*non-static(public)

Player::Player(const char *path) : AnimatedSprite(D_RENDERER, path, D_SCALE, D_RADIUS, D_FRAME_WIDTH, D_FRAME_HEIGHT, D_IMG_FRAMES, D_IMG_TYPES, D_ANIMATIONS_PER_SECOND),
								   Acceleration(D_ACCELERATION),
								   maxVelocity(D_MAX_VELOCITY),
								   Friction(D_FRICTION),
								   effectDuration(D_EFFECT_DURATION_SECONDS * 1000),
								   rotationSpeed(D_ROTATION_SPEED)
{
}

Player::~Player()
{
}

float Player::GetAcceleration()
{
	return Acceleration;
}

void Player::SetAcceleration(float acceleration)
{
	Acceleration = acceleration;

	return;
}

float Player::GetVelocity()
{
	return Velocity;
}

void Player::SetVelocity(float velocity)
{
	Velocity = velocity;

	return;
}

float Player::GetMaxVelocity()
{
	return maxVelocity;
}

void Player::SetMaxVelocity(float max_velocity)
{
	maxVelocity = max_velocity;

	return;
}

float Player::GetAngle()
{
	return Angle;
}

void Player::SetAngle(float angle)
{
	Angle = angle;

	return;
}

float Player::GetFriction()
{
	return Friction;
}

void Player::SetFriction(float friction)
{
	Friction = friction;

	return;
}

bool Player::GetGemCollected()
{
	return gemCollected;
}

void Player::SetGemCollected(bool new_status)
{
	gemCollected = new_status;

	return;
}

Uint64 Player::GetCollectionTicks()
{
	return collectionTicks;
}

void Player::SetCollectionTicks(Uint64 collection_ticks)
{
	collectionTicks = collection_ticks + effectDuration;

	return;
}

void Player::CheckType(Uint64 current_ticks)
{
	if (gemCollected && current_ticks >= collectionTicks)
	{
		gemCollected = false;
	}

	return;
}

void Player::HandleKeys(int screen_width, int screen_height, Uint64 delta_time_seconds, const Uint8 *keyboard)
{
	float pos_x = Rect.x;
	float pos_y = Rect.y;
	float width = Rect.w;
	float height = Rect.h;

	float angle = Angle;

	if (pos_x <= -width)
	{
		pos_x = screen_width;
	}
	else if (pos_x >= screen_width)
	{
		pos_x = -width;
	}
	if (pos_y <= -height)
	{
		pos_y = screen_height;
	}
	else if (pos_y >= screen_height)
	{
		pos_y = -height;
	}

	if (angle >= 360)
	{
		angle = 0;
	}
	else if (angle <= 0)
	{
		angle = 360 - angle;
	}

	if (keyboard[SDL_SCANCODE_LEFT])
	{
		angle -= rotationSpeed * delta_time_seconds;
	}
	if (keyboard[SDL_SCANCODE_RIGHT])
	{
		angle += rotationSpeed * delta_time_seconds;
	}
	if (keyboard[SDL_SCANCODE_UP])
	{
		Velocity += Acceleration * delta_time_seconds;
		gemCollected == false ? this->SetAnimationType(3) : this->SetAnimationType(4);
	}
	else
	{
		Velocity -= Friction * delta_time_seconds;
		gemCollected == false ? this->SetAnimationType(1) : this->SetAnimationType(2);
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

	pos_x += std::sin(angle * M_PI / 180) * Velocity * delta_time_seconds;
	pos_y -= std::cos(angle * M_PI / 180) * Velocity * delta_time_seconds;

	this->SetRectPos(pos_x, pos_y);
	Angle = angle;

	return;
}

void Player::Render()
{
	SDL_Rect srcrect = IMGPartRect;
	SDL_FRect destrect = Rect;
	SDL_RenderCopyExF(destRenderer, Texture, &srcrect, &destrect, Angle, nullptr, SDL_FLIP_NONE);

	return;
}