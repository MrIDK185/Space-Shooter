#ifndef GEM_HPP
#define GEM_HPP

#include "Configuration.hpp"
#include "AnimatedSprite.hpp"

#include <string>

class Gem : public AnimatedSprite
{
private:
	//*non-static

	std::string brightnessChange;

	Uint64
		blinkTicks,
		lifetimeTicks;

	unsigned int
		blinkDuration,
		lifetimeDuration,
		minimumBrightness,
		maximumBrightness,
		blinkFactor;

public:
	//*static

	static std::shared_ptr<Gem> NewGem(Configuration &config, SDL_Renderer *renderer, const char *path,
									   int screen_width, int screen_height);

	//*non-static

	Gem(SDL_Renderer *renderer, const char *path, float scale, float radius, const unsigned int frame_width,
		const unsigned int frame_height, const unsigned int img_frames, const unsigned int img_types,
		const unsigned int animations_per_second, unsigned int blink_duration, unsigned int lifetime_duration,
		unsigned int minimum_brightness, unsigned int maximum_brightness, unsigned int blink_factor,
		int screen_width, int screen_height);

	~Gem() override;

	std::string GetBrightnessChange() const;

	void SetBrightnessChange(const std::string &new_brightness);

	Uint64 GetBlinkTicks() const;

	void SetBlinkTicks(Uint64 new_blink_ticks);

	Uint64 GetLifetimeTicks() const;

	void SetLifetimeTicks(Uint64 new_lifetime_ticks);

	unsigned int GetBlinkDuration() const;

	void SetBlinkDuration(unsigned int new_duration);

	unsigned int GetLifetimeDuration() const;

	void SetLifetimeDuration(unsigned int new_duration);

	unsigned int GetMinimumBrightness() const;

	void SetMinimumBrightness(unsigned int minimum_brightness);

	unsigned int GetMaximumBrightness() const;

	void SetMaximumBrightness(unsigned int maximum_brightness);

	unsigned int GetBlinkFactor() const;

	void SetBlinkFactor(unsigned int blink_factor);

	void UpdateTicks(Uint64 current_ticks);

	void Blink();

	void Randomize(int screen_width, int screen_height, Uint64 current_ticks = 0);
};

#endif //! GEM_HPP