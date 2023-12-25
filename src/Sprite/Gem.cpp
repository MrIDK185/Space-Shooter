#include <random>

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
}

Gem::Gem(const Gem &obj)
	: AnimatedSprite(obj),
	  blinkTicks(obj.blinkTicks),
	  lifetimeTicks(obj.lifetimeTicks),
	  blinkDuration(obj.blinkDuration),
	  lifetimeDuration(obj.lifetimeDuration),
	  minimumBrightness(obj.minimumBrightness),
	  maximumBrightness(obj.maximumBrightness),
	  blinkFactor(obj.blinkFactor),
	  signedFactor(obj.signedFactor)
{
}

Gem::Gem(Gem &&obj)
	: AnimatedSprite(std::move(obj)),
	  blinkTicks(obj.blinkTicks),
	  lifetimeTicks(obj.lifetimeTicks),
	  blinkDuration(obj.blinkDuration),
	  lifetimeDuration(obj.lifetimeDuration),
	  minimumBrightness(obj.minimumBrightness),
	  maximumBrightness(obj.maximumBrightness),
	  blinkFactor(obj.blinkFactor),
	  signedFactor(obj.signedFactor)
{
}

Uint64 Gem::GetBlinkTicks() const
{
	return blinkTicks;
}

void Gem::SetBlinkTicks(Uint64 new_blink_ticks)
{
	blinkTicks = new_blink_ticks;

	return;
}

Uint64 Gem::GetLifetimeTicks() const
{
	return lifetimeTicks;
}

void Gem::SetLifetimeTicks(Uint64 new_lifetime_ticks)
{
	lifetimeTicks = new_lifetime_ticks;

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

Sign Gem::GetSignedFactor() const
{
	return signedFactor;
}

void Gem::SetSignedFactor(Sign new_sign)
{
	signedFactor = new_sign;

	return;
}

void Gem::UpdateTicks(Uint64 current_ticks)
{
	blinkTicks = current_ticks + blinkDuration;
	lifetimeTicks = current_ticks + lifetimeDuration;

	return;
}

void Gem::Blink(float delta_time_seconds)
{
	Uint8 brightness_value;
	SDL_GetTextureColorMod(Texture.get(), &brightness_value, nullptr, nullptr);
	float next_brightness_value = static_cast<float>(brightness_value);

	next_brightness_value += static_cast<float>(blinkFactor) * static_cast<float>(signedFactor) * delta_time_seconds;

	if (next_brightness_value >= maximumBrightness)
	{
		next_brightness_value = maximumBrightness;
		signedFactor = DECREMENT;
	}
	if (next_brightness_value <= minimumBrightness)
	{
		next_brightness_value = minimumBrightness;
		signedFactor = INCREMENT;
	}

	SDL_SetTextureColorMod(Texture.get(), next_brightness_value, next_brightness_value, next_brightness_value);

	return;
}

void Gem::Randomize(int screen_width, int screen_height, Uint64 current_ticks)
{
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution<> random_type(1, IMG_TYPES);
	std::uniform_int_distribution<> random_frame(1, IMG_FRAMES);
	std::uniform_int_distribution<> random_x(0, screen_width - IMGPartRect.w);
	std::uniform_int_distribution<> random_y(0, screen_height - IMGPartRect.h);

	int new_type = random_type(gen);
	int new_frame = random_frame(gen);
	int new_x = random_x(gen);
	int new_y = random_y(gen);

	SetAnimationType(new_type);
	SetAnimationFrame(new_frame);
	SetRectPos(new_x, new_y);

	UpdateTicks(current_ticks);
	SDL_SetTextureColorMod(Texture.get(), 255, 255, 255);

	return;
}