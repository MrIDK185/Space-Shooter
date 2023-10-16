#ifndef ANIMATED_SPRITE_HPP
#define ANIMATED_SPRITE_HPP

#include "IMGSprite.hpp"

class AnimatedSprite : public IMGSprite
{
protected:
	//*non-static

	const unsigned int
		FRAME_WIDTH,
		FRAME_HEIGHT,
		IMG_FRAMES,
		IMG_TYPES,
		ANIMATIONS_PER_SECOND;

	unsigned int
		animationFrame = 1,
		animationType = 1;

	SDL_Rect IMGPartRect;
	Uint64 nextTickTime;

public:
	//* non-static

	AnimatedSprite(SDL_Renderer *renderer, std::string path, float scale, float radius,
				   const unsigned int frame_width, const unsigned int frame_height,
				   const unsigned int img_frames, const unsigned int img_types, const unsigned int animations_per_second);

	~AnimatedSprite() override;

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

	Uint64 GetNextTickTime() const;

	void SetNextTickTime(Uint64 new_time);

	void Animate(Uint64 current_ticks);

	void Render() const override;
};

#endif //! ANIMATED_SPRITE_HPP