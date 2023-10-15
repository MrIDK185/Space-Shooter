#ifndef GEM_HPP
#define GEM_HPP

#include "AnimatedSprite.hpp"

#include <string>

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

public:
	//*static

	static void InitDefaultValues(SDL_Renderer *renderer, const char *path, float scale, float radius, const unsigned int frame_width, const unsigned int frame_height, const unsigned int img_frames, const unsigned int img_types, const unsigned int animations_per_second, unsigned int blink_duration, unsigned int lifetime_duration);

	static void CleanDefaultValues();

	static std::shared_ptr<Gem> NewGem(int screen_width, int screen_height);

	//*non-static

	Gem(SDL_Renderer *renderer, const char *path, float scale, float radius, const unsigned int frame_width, const unsigned int frame_height, const unsigned int img_frames, const unsigned int img_types, const unsigned int animations_per_second, unsigned int blink_duration, unsigned int lifetime_duration, int screen_width, int screen_height);

	~Gem() override;

	std::string GetBrightnessChange() const;

	void SetBrightnessChange(const std::string &new_brightness);

	Uint64 GetBlinkTicks() const;

	void SetBlinkTicks(Uint64 new_blink_ticks);

	unsigned int GetBlinkDuration() const;

	void SetLifetimeTicks(Uint64 new_lifetime_ticks);

	unsigned int GetLifetimeDuration() const;

	void SetBlinkDuration(unsigned int new_duration);

	Uint64 GetLifetimeTicks() const;

	void SetLifetimeDuration(unsigned int new_duration);

	void SetTicks(Uint64 current_ticks);

	void Blink(int gem_minimum_brightness, int gem_maximum_brightness, int gem_blink_factor);

	void Randomize(int screen_width, int screen_height, Uint64 current_ticks = 0);
};

#endif //! GEM_HPP