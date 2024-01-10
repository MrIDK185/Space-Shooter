#include <random>
#include <cmath>

#include "Sprite/Gem.hpp"

//* non-static(public)

Gem::Gem(SpriteData sprite_data, AnimationData animation_data, GemData gem_data)
	: AnimatedSprite(sprite_data, animation_data),
	  blinkDurationMilliseconds(gem_data.blinkDurationMilliseconds),
	  lifetimeDurationMilliseconds(gem_data.lifetimeDurationMilliseconds),
	  minimumBrightness(gem_data.minimumBrightness),
	  maximumBrightness(gem_data.maximumBrightness),
	  blinkFactor(gem_data.blinkFactor)
{
	blinkTimer = Timer(NO_EVENT, GEM_BLINK, blinkDurationMilliseconds, blinkDurationMilliseconds,
					   gem_data.game_state, this);
	lifetimeTimer = Timer(NO_EVENT, GEM_DISAPPEAR, lifetimeDurationMilliseconds, lifetimeDurationMilliseconds,
						  gem_data.game_state, this);

	return;
}

Gem::Gem(const Gem &obj)
	: AnimatedSprite(obj),
	  Blinking(obj.Blinking),
	  blinkAngle(obj.blinkAngle),
	  blinkDurationMilliseconds(obj.blinkDurationMilliseconds),
	  lifetimeDurationMilliseconds(obj.lifetimeDurationMilliseconds),
	  minimumBrightness(obj.minimumBrightness),
	  maximumBrightness(obj.maximumBrightness),
	  blinkFactor(obj.blinkFactor)
{
	blinkTimer = Timer(NO_EVENT, GEM_BLINK, blinkDurationMilliseconds, blinkDurationMilliseconds,
					   obj.blinkTimer.currentGameState, this);
	lifetimeTimer = Timer(NO_EVENT, GEM_DISAPPEAR, lifetimeDurationMilliseconds, lifetimeDurationMilliseconds,
						  obj.lifetimeTimer.currentGameState, this);

	return;
}

Gem::Gem(Gem &&obj)
	: AnimatedSprite(std::move(obj)),
	  Blinking(obj.Blinking),
	  blinkAngle(obj.blinkAngle),
	  blinkDurationMilliseconds(obj.blinkDurationMilliseconds),
	  lifetimeDurationMilliseconds(obj.lifetimeDurationMilliseconds),
	  minimumBrightness(obj.minimumBrightness),
	  maximumBrightness(obj.maximumBrightness),
	  blinkFactor(obj.blinkFactor)
{
	blinkTimer = Timer(NO_EVENT, GEM_BLINK, blinkDurationMilliseconds, blinkDurationMilliseconds,
					   obj.blinkTimer.currentGameState, this);
	lifetimeTimer = Timer(NO_EVENT, GEM_DISAPPEAR, lifetimeDurationMilliseconds, lifetimeDurationMilliseconds,
						  obj.lifetimeTimer.currentGameState, this);

	return;
}

bool Gem::GetBlinking() const
{
	return Blinking;
}

void Gem::SetBlinking(bool blinking)
{
	Blinking = blinking;

	return;
}

float Gem::GetBlinkAngle() const
{
	return blinkAngle;
}

void Gem::SetBlinkAngle(float new_angle)
{
	blinkAngle = new_angle;

	return;
}

unsigned int Gem::GetBlinkDuration() const
{
	return blinkDurationMilliseconds;
}

void Gem::SetBlinkDuration(unsigned int new_duration_milliseconds)
{
	blinkDurationMilliseconds = new_duration_milliseconds;

	return;
}

unsigned int Gem::GetLifetimeDuration() const
{
	return lifetimeDurationMilliseconds;
}

void Gem::SetLifetimeDuration(unsigned int new_duration_milliseconds)
{
	lifetimeDurationMilliseconds = new_duration_milliseconds;

	return;
}

unsigned int Gem::GetMinimumBrightness() const
{
	return minimumBrightness;
}

void Gem::SetMinimumBrightness(unsigned int minimum_brightness)
{
	minimumBrightness = minimum_brightness;

	return;
}

unsigned int Gem::GetMaximumBrightness() const
{
	return maximumBrightness;
}

void Gem::SetMaximumBrightness(unsigned int maximum_brightness)
{
	maximumBrightness = maximum_brightness;

	return;
}

unsigned int Gem::GetBlinkFactor() const
{
	return blinkFactor;
}

void Gem::SetBlinkFactor(unsigned int blink_factor)
{
	blinkFactor = blink_factor;

	return;
}

void Gem::Blink(float delta_time_seconds)
{
	if (!Blinking)
	{
		return;
	}

	float amplitude = (maximumBrightness - minimumBrightness) / 2;
	float brightness = amplitude * std::cos(blinkAngle) + minimumBrightness + amplitude;

	SDL_SetTextureColorMod(Texture.get(), brightness, brightness, brightness);

	blinkAngle += (blinkFactor / 10) * M_PI * delta_time_seconds;

	return;
}

void Gem::StopTimers()
{
	blinkTimer.Stop();
	lifetimeTimer.Stop();

	return;
}

void Gem::RestartTimers()
{
	blinkTimer.Restart();
	lifetimeTimer.Restart();

	return;
}

void Gem::Randomize(int screen_width, int screen_height)
{
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution<> random_type(1, IMG_TYPES);
	std::uniform_int_distribution<> random_frame(1, IMG_FRAMES);
	std::uniform_int_distribution<> random_x(0, screen_width - FRAME_WIDTH);
	std::uniform_int_distribution<> random_y(0, screen_height - FRAME_HEIGHT);

	SetAnimationType(random_type(gen));
	SetAnimationFrame(random_frame(gen));
	SetRectPos(random_x(gen), random_y(gen), NW);

	SDL_SetTextureColorMod(Texture.get(), 255, 255, 255);

	Blinking = false;
	blinkAngle = 0;

	return;
}