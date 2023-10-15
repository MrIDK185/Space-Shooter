#include "Gem.hpp"

#include <random>

//*static(public)

std::shared_ptr<Gem> Gem::NewGem(Configuration &config, SDL_Renderer *renderer, const char *path, int screen_width, int screen_height)
{
	return std::make_shared<Gem>(renderer, path, config.GEM_SCALE, config.GEM_RADIUS, config.GEM_FRAME_WIDTH,
								 config.GEM_FRAME_HEIGHT, config.GEM_IMG_FRAMES, config.GEM_IMG_TYPES, 0, config.GEM_BLINK_DURATION,
								 config.GEM_LIFETIME_DURATION, screen_width, screen_height);
}

//*non-static(public)

Gem::Gem(SDL_Renderer *renderer, const char *path, float scale, float radius, const unsigned int frame_width,
		 const unsigned int frame_height, const unsigned int img_frames, const unsigned int img_types,
		 const unsigned int animations_per_second, unsigned int blink_duration, unsigned int lifetime_duration,
		 int screen_width, int screen_height)
	: AnimatedSprite(renderer, path, scale, radius, frame_width, frame_height, img_frames, img_types, animations_per_second),
	  blinkDuration(blink_duration),
	  lifetimeDuration(lifetime_duration)
{
	Randomize(screen_width, screen_height, SDL_GetTicks64());

	return;
}

Gem::~Gem()
{
}

std::string Gem::GetBrightnessChange() const
{
	return brightnessChange;
}

void Gem::SetBrightnessChange(const std::string &new_brightness)
{
	brightnessChange = new_brightness;

	return;
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

unsigned int Gem::GetBlinkDuration() const
{
	return blinkDuration;
}

void Gem::SetBlinkDuration(unsigned int new_duration)
{
	blinkDuration = new_duration;

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

unsigned int Gem::GetLifetimeDuration() const
{
	return lifetimeDuration;
}

void Gem::SetLifetimeDuration(unsigned int new_duration)
{
	lifetimeDuration = new_duration;

	return;
}

void Gem::SetTicks(Uint64 current_ticks)
{
	blinkTicks = current_ticks + blinkDuration;
	lifetimeTicks = current_ticks + lifetimeDuration;

	return;
}

void Gem::Blink(int gem_minimum_brightness, int gem_maximum_brightness, int gem_blink_factor)
{
	Uint8 brightness_value;
	SDL_GetTextureColorMod(Texture, &brightness_value, nullptr, nullptr);
	Uint8 next_brightness_value = brightness_value;

	if (brightness_value >= gem_maximum_brightness)
	{
		brightnessChange = "Down";
	}
	if (brightness_value <= gem_minimum_brightness)
	{
		brightnessChange = "Up";
	}

	if (brightnessChange == "Up")
	{
		next_brightness_value = brightness_value + gem_blink_factor;
	}
	else if (brightnessChange == "Down")
	{
		next_brightness_value = brightness_value - gem_blink_factor;
	}

	SDL_SetTextureColorMod(Texture, next_brightness_value, next_brightness_value, next_brightness_value);

	return;
}

void Gem::Randomize(int screen_width, int screen_height, Uint64 current_ticks)
{
	SDL_Rect sprite_rect = IMGPartRect;

	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution<> random_type(1, IMG_TYPES);
	std::uniform_int_distribution<> random_frame(1, IMG_FRAMES);
	std::uniform_int_distribution<> random_x(0, screen_width - sprite_rect.w);
	std::uniform_int_distribution<> random_y(0, screen_height - sprite_rect.h);

	int new_type = random_type(gen);
	int new_frame = random_frame(gen);
	int new_x = random_x(gen);
	int new_y = random_y(gen);

	SetAnimationType(new_type);
	SetAnimationFrame(new_frame);
	SetRectPos(new_x, new_y);

	blinkTicks = current_ticks + blinkDuration;
	lifetimeTicks = current_ticks + lifetimeDuration;
	SDL_SetTextureColorMod(Texture, 255, 255, 255);

	return;
}