#include "Sprite/AnimatedSprite.hpp"

//* non-static(public)

AnimatedSprite::AnimatedSprite(SpriteData sprite_data, AnimationData animation_data)
	: IMGSprite(sprite_data),
	  FRAME_WIDTH(animation_data.FRAME_WIDTH),
	  FRAME_HEIGHT(animation_data.FRAME_HEIGHT),
	  IMG_FRAMES(animation_data.IMG_FRAMES),
	  IMG_TYPES(animation_data.IMG_TYPES),
	  ANIMATIONS_PER_SECOND(animation_data.ANIMATIONS_PER_SECOND),
	  IMGPartRect({0, 0, (int)FRAME_WIDTH, (int)FRAME_HEIGHT})
{
	SetRectSize(FRAME_WIDTH * Scale, FRAME_HEIGHT * Scale);

	return;
}

AnimatedSprite::AnimatedSprite(const AnimatedSprite &obj)
	: IMGSprite(obj),
	  FRAME_WIDTH(obj.FRAME_WIDTH),
	  FRAME_HEIGHT(obj.FRAME_HEIGHT),
	  IMG_FRAMES(obj.IMG_FRAMES),
	  IMG_TYPES(obj.IMG_TYPES),
	  ANIMATIONS_PER_SECOND(obj.ANIMATIONS_PER_SECOND),
	  animationFrame(obj.animationFrame),
	  animationType(obj.animationType),
	  IMGPartRect(obj.IMGPartRect),
	  nextTickTime(obj.nextTickTime)
{
}

AnimatedSprite::AnimatedSprite(AnimatedSprite &&obj)
	: IMGSprite(std::move(obj)),
	  FRAME_WIDTH(obj.FRAME_WIDTH),
	  FRAME_HEIGHT(obj.FRAME_HEIGHT),
	  IMG_FRAMES(obj.IMG_FRAMES),
	  IMG_TYPES(obj.IMG_TYPES),
	  ANIMATIONS_PER_SECOND(obj.ANIMATIONS_PER_SECOND),
	  animationFrame(obj.animationFrame),
	  animationType(obj.animationType),
	  IMGPartRect(obj.IMGPartRect),
	  nextTickTime(obj.nextTickTime)
{
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
		pos_x -= FRAME_WIDTH;
		break;
	case SE:
		pos_x -= FRAME_WIDTH;
		pos_y -= FRAME_HEIGHT;
		break;
	case SW:
		pos_y -= FRAME_HEIGHT;
		break;
	case CENTER:
		pos_x -= FRAME_WIDTH / 2;
		pos_y -= FRAME_HEIGHT / 2;
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
	if (current_ticks < nextTickTime || ANIMATIONS_PER_SECOND <= 0)
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
	SDL_RenderCopyF(*destRenderer, Texture.get(), &IMGPartRect, &Rect);

	return;
}