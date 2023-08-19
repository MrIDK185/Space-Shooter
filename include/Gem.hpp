#ifndef GEM_HPP
#define GEM_HPP

#include "AnimatedSprite.hpp"

class Gem : public AnimatedSprite
{
private:
	//*static

	static SDL_Renderer *D_RENDERER;
	static const char *D_PATH;

	static float
		D_SCALE,
		D_RADIUS;

	static unsigned int
		D_FRAME_WIDTH,
		D_FRAME_HEIGHT,
		D_IMG_FRAMES,
		D_IMG_TYPES,
		D_ANIMATIONS_PER_SECOND;

	static Uint64
		D_BLINKDURATION,
		D_LIFETIMEDURATION;

	//*non-static

	std::string brightnessChange;

	Uint64
		blinkTicks,
		lifetimeTicks;

	unsigned int
		blinkDuration,
		lifetimeDuration;

	Configuration &Config;

public:
	//*static

	static void InitDefaultValues(SDL_Renderer *renderer, const char *path, float scale, float radius, const unsigned int frame_width, const unsigned int frame_height, const unsigned int img_frames, const unsigned int img_types, const unsigned int animations_per_second, unsigned int blink_duration, unsigned int lifetime_duration);

	static void CleanDefaultValues();

	static std::shared_ptr<Gem> NewGem(Configuration &config);

	//*non-static

	Gem(SDL_Renderer *renderer, const char *path, float scale, float radius, const unsigned int frame_width, const unsigned int frame_height, const unsigned int img_frames, const unsigned int img_types, const unsigned int animations_per_second, unsigned int blink_duration, unsigned int lifetime_duration, Configuration &config);

	~Gem() override;

	std::string GetBrightnessChange();

	void SetBrightnessChange(const std::string &new_brightness);

	Uint64 GetBlinkTicks();

	void SetBlinkTicks(Uint64 new_blink_ticks);

	unsigned int GetBlinkDuration();

	void SetLifetimeTicks(Uint64 new_lifetime_ticks);

	unsigned int GetLifetimeDuration();

	void SetBlinkDuration(unsigned int new_duration);

	Uint64 GetLifetimeTicks();

	void SetLifetimeDuration(unsigned int new_duration);

	void SetTicks(Uint64 current_ticks);

	void Blink(unsigned int factor, unsigned int max_brightness, unsigned int min_brightness);

	void Randomize(Uint64 current_ticks);
};

#endif //! GEM_HPP