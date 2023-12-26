#ifndef GEM_HPP
#define GEM_HPP

#include "Sprite/AnimatedSprite.hpp"
#include "Sprite/GemData.hpp"
#include "Sprite/Sign.hpp"

class Gem : public AnimatedSprite
{
private:
	//* non-static

	Uint64
		blinkTicks = 0,
		lifetimeTicks = 0;

	unsigned int
		blinkDuration,
		lifetimeDuration,
		minimumBrightness,
		maximumBrightness,
		blinkFactor;

	Sign signedFactor = DECREMENT;

public:
	//* non-static

	Gem(SpriteData sprite_data, AnimationData animation_data, GemData gem_data);

	explicit Gem(const Gem &obj);

	explicit Gem(Gem &&obj);

	~Gem() override = default;

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

	Sign GetSignedFactor() const;

	void SetSignedFactor(Sign new_sign);

	void UpdateTicks(Uint64 current_ticks);

	void Blink(float delta_time_seconds);

	void Randomize(int screen_width, int screen_height, Uint64 current_ticks = 0);
};

#endif //! GEM_HPP