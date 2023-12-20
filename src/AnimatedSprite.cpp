#include "AnimatedSprite.hpp"

//*non-static(public)

AnimatedSprite::AnimatedSprite(SDL_Renderer *renderer, std::string path, float scale, float radius, const unsigned int frame_width,
							   const unsigned int frame_height, const unsigned int img_frames, const unsigned int img_types,
							   const unsigned int animations_per_second)
	: IMGSprite::IMGSprite(renderer, path, scale, radius),
	  FRAME_WIDTH(frame_width),
	  FRAME_HEIGHT(frame_height),
	  IMG_FRAMES(img_frames),
	  IMG_TYPES(img_types),
	  ANIMATIONS_PER_SECOND(animations_per_second),
	  IMGPartRect({0, 0, (int)frame_width, (int)frame_height}),
	  nextTickTime(0)
{
	SetRectSize(frame_width * scale, frame_height * scale);

	return;
}

unsigned int AnimatedSprite::GetFrameWidth() const
{
	return FRAME_WIDTH;
}

unsigned int AnimatedSprite::GetFrameHeight() const
{
	return FRAME_HEIGHT;
}

unsigned int AnimatedSprite::GetIMGFrames() const
{
	return IMG_FRAMES;
}

unsigned int AnimatedSprite::GetIMGTypes() const
{
	return IMG_TYPES;
}

unsigned int AnimatedSprite::GetAnimationsPerSecond() const
{
	return ANIMATIONS_PER_SECOND;
}

unsigned int AnimatedSprite::GetAnimationFrame() const
{
	return animationFrame;
}

void AnimatedSprite::SetAnimationFrame(unsigned int frame)
{
	if (frame > IMG_FRAMES)
	{
		return;
	}

	IMGPartRect.x = (frame - 1) * FRAME_WIDTH;
	animationFrame = frame;

	return;
}

unsigned int AnimatedSprite::GetAnimationType() const
{
	return animationType;
}

void AnimatedSprite::SetAnimationType(unsigned int type)
{
	if (type > IMG_TYPES)
	{
		return;
	}

	IMGPartRect.y = (type - 1) * FRAME_HEIGHT;
	animationType = type;

	return;
}

SDL_Rect AnimatedSprite::GetIMGPartRect() const
{
	return IMGPartRect;
}

void AnimatedSprite::SetRectPos(float pos_x, float pos_y, Alignments align)
{
	switch (align)
	{
	case NW:
		break;
	case NE:
		pos_x -= IMGPartRect.w;
		break;
	case SE:
		pos_x -= IMGPartRect.w;
		pos_y -= IMGPartRect.h;
		break;
	case SW:
		pos_y -= IMGPartRect.h;
		break;
	case CENTER:
		pos_x -= IMGPartRect.w / 2;
		pos_y -= IMGPartRect.h / 2;
		break;
	default:
		break;
	}

	Rect.x = pos_x;
	Rect.y = pos_y;

	return;
}

Uint64 AnimatedSprite::GetNextTickTime() const
{
	return nextTickTime;
}

void AnimatedSprite::SetNextTickTime(Uint64 new_time)
{
	nextTickTime = new_time;

	return;
}

void AnimatedSprite::Animate(Uint64 current_ticks)
{
	if (current_ticks < nextTickTime | ANIMATIONS_PER_SECOND <= 0)
	{
		return;
	}

	if (animationFrame == IMG_FRAMES)
	{
		SetAnimationFrame(1);
	}
	else
	{
		SetAnimationFrame(animationFrame + 1);
	}

	SetNextTickTime(current_ticks + 1000 / ANIMATIONS_PER_SECOND);

	return;
}

void AnimatedSprite::Render() const
{
	SDL_RenderCopyF(destRenderer, Texture.get(), &IMGPartRect, &Rect);

	return;
}