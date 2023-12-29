#ifndef GEM_HPP
#define GEM_HPP

#include "Sprite/AnimatedSprite.hpp"
#include "Sprite/GemData.hpp"
#include "Time/Timer.hpp"

class Gem : public AnimatedSprite
{
private:
	//* non-static

	bool Blinking = false;
	float blinkAngle = 0;

	unsigned int
		blinkDurationMilliseconds,
		lifetimeDurationMilliseconds,
		minimumBrightness,
		maximumBrightness,
		blinkFactor;

	Timer
		blinkTimer,
		lifetimeTimer;

public:
	//* non-static

	Gem(SpriteData sprite_data, AnimationData animation_data, GemData gem_data);

	explicit Gem(const Gem &obj);

	explicit Gem(Gem &&obj);

	~Gem() override = default;

	bool GetBlinking() const;

	void SetBlinking(bool blinking);

	float GetBlinkAngle() const;

	void SetBlinkAngle(float new_angle);

	unsigned int GetBlinkDuration() const;

	void SetBlinkDuration(unsigned int new_duration_milliseconds);

	unsigned int GetLifetimeDuration() const;

	void SetLifetimeDuration(unsigned int new_duration_milliseconds);

	unsigned int GetMinimumBrightness() const;

	void SetMinimumBrightness(unsigned int minimum_brightness);

	unsigned int GetMaximumBrightness() const;

	void SetMaximumBrightness(unsigned int maximum_brightness);

	unsigned int GetBlinkFactor() const;

	void SetBlinkFactor(unsigned int blink_factor);

	void Blink(float delta_time_seconds);

	void StopTimers();

	void Randomize(int screen_width, int screen_height);
};

#endif //! GEM_HPP