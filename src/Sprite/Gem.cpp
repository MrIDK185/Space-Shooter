#include <random>
#include <cmath>

#include "Sprite/Gem.hpp"

//* non-static(public)

Gem::Gem(SpriteData sprite_data, AnimationData animation_data, GemData gem_data)
	: AnimatedSprite(sprite_data, animation_data),
	  blinkDuration(gem_data.blinkDuration),
	  lifetimeDuration(gem_data.lifetimeDuration),
	  minimumBrightness(gem_data.minimumBrightness),
	  maximumBrightness(gem_data.maximumBrightness),
	  blinkFactor(gem_data.blinkFactor)
{
	blinkTimer = Timer(NO_EVENT, GEM_BLINK, blinkDuration, blinkDuration,
					   gem_data.game_state, this);
	lifetimeTimer = Timer(NO_EVENT, GEM_DISAPPEAR, lifetimeDuration, lifetimeDuration,
						  gem_data.game_state, this);

	return;
}

Gem::Gem(const Gem &obj)
	: AnimatedSprite(obj),
	  Blinking(obj.Blinking),
	  blinkAngle(obj.blinkAngle),
	  blinkDuration(obj.blinkDuration),
	  lifetimeDuration(obj.lifetimeDuration),
	  minimumBrightness(obj.minimumBrightness),
	  maximumBrightness(obj.maximumBrightness),
	  blinkFactor(obj.blinkFactor)
{
	blinkTimer = Timer(NO_EVENT, GEM_BLINK, blinkDuration, blinkDuration,
					   obj.blinkTimer.currentGameState, this);
	lifetimeTimer = Timer(NO_EVENT, GEM_DISAPPEAR, lifetimeDuration, lifetimeDuration,
						  obj.lifetimeTimer.currentGameState, this);

	return;
}

Gem::Gem(Gem &&obj)
	: AnimatedSprite(std::move(obj)),
	  Blinking(obj.Blinking),
	  blinkAngle(obj.blinkAngle),
	  blinkDuration(obj.blinkDuration),
	  lifetimeDuration(obj.lifetimeDuration),
	  minimumBrightness(obj.minimumBrightness),
	  maximumBrightness(obj.maximumBrightness),
	  blinkFactor(obj.blinkFactor)
{
	blinkTimer = Timer(NO_EVENT, GEM_BLINK, blinkDuration, blinkDuration,
					   obj.blinkTimer.currentGameState, this);
	lifetimeTimer = Timer(NO_EVENT, GEM_DISAPPEAR, lifetimeDuration, lifetimeDuration,
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
	return blinkDuration;
}

void Gem::SetBlinkDuration(unsigned int new_duration)
{
	blinkDuration = new_duration;

	return;
}

unsigned int Gem::GetLifetimeDuration() const
{
	return lifetimeDuration;
}

void Gem::SetLifetimeDuration(unsigned int new_duration)
{
	lifetimeDuration = new_duration;

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

	blinkAngle += blinkFactor * M_PI * delta_time_seconds;

	return;
}

void Gem::StopTimers()
{
	blinkTimer.Stop();
	lifetimeTimer.Stop();

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

	blinkTimer.Restart();
	lifetimeTimer.Restart();

	Blinking = false;
	blinkAngle = 0;

	return;
}