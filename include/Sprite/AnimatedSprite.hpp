#ifndef ANIMATED_SPRITE_HPP
#define ANIMATED_SPRITE_HPP

#include "Sprite/IMGSprite.hpp"
#include "Sprite/AnimationData.hpp"

class AnimatedSprite : public IMGSprite
{
protected:
	//* non-static

	const unsigned int
		FRAME_WIDTH = 0,
		FRAME_HEIGHT = 0,
		IMG_FRAMES = 0,
		IMG_TYPES = 0,
		ANIMATIONS_PER_SECOND = 0;

	unsigned int
		animationFrame = 1,
		animationType = 1;

	SDL_Rect IMGPartRect;

	Uint64 nextTickTime = 0;

public:
	//* non-static

	AnimatedSprite(SpriteData sprite_data, AnimationData animation_data);

	explicit AnimatedSprite(const AnimatedSprite &obj);

	explicit AnimatedSprite(AnimatedSprite &&obj);

	explicit AnimatedSprite() = default;

	~AnimatedSprite() override = default;

	unsigned int GetFrameWidth() const;

	unsigned int GetFrameHeight() const;

	unsigned int GetIMGFrames() const;

	unsigned int GetIMGTypes() const;

	unsigned int GetAnimationsPerSecond() const;

	unsigned int GetAnimationFrame() const;

	void SetAnimationFrame(unsigned int frame);

	unsigned int GetAnimationType() const;

	void SetAnimationType(unsigned int type);

	SDL_Rect GetIMGPartRect() const;

	void SetRectPos(float pos_x, float pos_y, Alignments align = CENTER) override;

	Uint64 GetNextTickTime() const;

	void SetNextTickTime(Uint64 new_time);

	void Animate(Uint64 current_ticks);

	void Render() const override;
};

#endif //! ANIMATED_SPRITE_HPP