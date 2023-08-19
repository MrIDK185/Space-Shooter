#include "Globals.hpp"
#include "Gem.hpp"

#include <random>

//*static(private)

SDL_Renderer *Gem::D_RENDERER = nullptr;
const char *Gem::D_PATH = nullptr;

float
	Gem::D_SCALE = 0,
	Gem::D_RADIUS = 0;

unsigned int
	Gem::D_FRAME_WIDTH = 0,
	Gem::D_FRAME_HEIGHT = 0,
	Gem::D_IMG_FRAMES = 0,
	Gem::D_IMG_TYPES = 0,
	Gem::D_ANIMATIONS_PER_SECOND = 0;

Uint64
	Gem::D_BLINKDURATION = 0,
	Gem::D_LIFETIMEDURATION = 0;

//*static(public)

void Gem::InitDefaultValues(SDL_Renderer *renderer, const char *path, float scale, float radius, const unsigned int frame_width, const unsigned int frame_height, const unsigned int img_frames, const unsigned int img_types, const unsigned int animations_per_second, unsigned int blink_duration, unsigned int lifetime_duration)
{
	D_RENDERER = renderer;
	D_PATH = path;
	D_SCALE = scale;
	D_RADIUS = radius;
	D_FRAME_WIDTH = frame_width;
	D_FRAME_HEIGHT = frame_height;
	D_IMG_FRAMES = img_frames;
	D_IMG_TYPES = img_types;
	D_ANIMATIONS_PER_SECOND = animations_per_second;
	D_BLINKDURATION = blink_duration;
	D_LIFETIMEDURATION = lifetime_duration;

	return;
}

void Gem::CleanDefaultValues()
{
	D_RENDERER = nullptr;
	D_PATH = nullptr;

	return;
}

std::shared_ptr<Gem> Gem::NewGem(Configuration &config)
{
	return std::make_shared<Gem>(D_RENDERER, D_PATH, D_SCALE, D_RADIUS, D_FRAME_WIDTH, D_FRAME_HEIGHT, D_IMG_FRAMES, D_IMG_TYPES, D_ANIMATIONS_PER_SECOND, D_BLINKDURATION, D_LIFETIMEDURATION, config);
}

//*non-static(public)

Gem::Gem(SDL_Renderer *renderer, const char *path, float scale, float radius, const unsigned int frame_width, const unsigned int frame_height, const unsigned int img_frames, const unsigned int img_types, const unsigned int animations_per_second, unsigned int blink_duration, unsigned int lifetime_duration, Configuration &config) : AnimatedSprite(renderer, path, scale, radius, frame_width, frame_height, img_frames, img_types, animations_per_second),
																																																																																			blinkDuration(blink_duration),
																																																																																			lifetimeDuration(lifetime_duration),
																																																																																			Config(config)
{
	this->Randomize(SDL_GetTicks64());

	return;
}

Gem::~Gem()
{
}

std::string Gem::GetBrightnessChange()
{
	return brightnessChange;
}

void Gem::SetBrightnessChange(const std::string &new_brightness)
{
	brightnessChange = new_brightness;

	return;
}

Uint64 Gem::GetBlinkTicks()
{
	return blinkTicks;
}

void Gem::SetBlinkTicks(Uint64 new_blink_ticks)
{
	blinkTicks = new_blink_ticks;

	return;
}

unsigned int Gem::GetBlinkDuration()
{
	return blinkDuration;
}

void Gem::SetBlinkDuration(unsigned int new_duration)
{
	blinkDuration = new_duration;

	return;
}

Uint64 Gem::GetLifetimeTicks()
{
	return lifetimeTicks;
}

void Gem::SetLifetimeTicks(Uint64 new_lifetime_ticks)
{
	lifetimeTicks = new_lifetime_ticks;

	return;
}

unsigned int Gem::GetLifetimeDuration()
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

void Gem::Blink(unsigned int factor, unsigned int max_brightness, unsigned int min_brightness)
{
	Uint8 brightness_value;
	SDL_GetTextureColorMod(Texture, &brightness_value, nullptr, nullptr);
	Uint8 next_brightness_value = brightness_value;

	if (brightness_value >= max_brightness)
	{
		brightnessChange = "Down";
	}
	if (brightness_value <= min_brightness)
	{
		brightnessChange = "Up";
	}

	if (brightnessChange == "Up")
	{
		next_brightness_value = brightness_value + factor;
	}
	else if (brightnessChange == "Down")
	{
		next_brightness_value = brightness_value - factor;
	}

	SDL_SetTextureColorMod(Texture, next_brightness_value, next_brightness_value, next_brightness_value);

	return;
}

void Gem::Randomize(Uint64 current_ticks)
{
	SDL_Rect sprite_rect = IMGPartRect;

	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution<> random_type(1, IMG_TYPES);
	std::uniform_int_distribution<> random_frame(1, IMG_FRAMES);
	std::uniform_int_distribution<> random_x(0, Config.SCREEN_RESOLUTION_WIDTH - sprite_rect.w);
	std::uniform_int_distribution<> random_y(0, Config.SCREEN_RESOLUTION_HEIGHT - sprite_rect.h);

	int new_type = random_type(gen);
	int new_frame = random_frame(gen);
	int new_x = random_x(gen);
	int new_y = random_y(gen);

	this->SetAnimationType(new_type);
	this->SetAnimationFrame(new_frame);
	this->SetRectPos(new_x, new_y);

	blinkTicks = current_ticks + blinkDuration;
	lifetimeTicks = current_ticks + lifetimeDuration;
	SDL_SetTextureColorMod(Texture, 255, 255, 255);

	return;
}