#include "Sprite/IMGSprite.hpp"

#include <SDL2/SDL_image.h>

//* non-static(protected)

void IMGSprite::LoadImage(float scale /*1*/)
{
	SetSurface(IMG_Load(IMGPath.c_str()));
	SetTexture(SDL_CreateTextureFromSurface(*destRenderer, Surface.get()));

	int width, height;
	SDL_QueryTexture(Texture.get(), nullptr, nullptr, &width, &height);
	SetRectSize(width, height);
	SetScale(scale);

	return;
}

//* non-static(public)

IMGSprite::IMGSprite(SpriteData sprite_data)
	: Sprite(sprite_data.destRenderer),
	  IMGPath(sprite_data.IMGPath),
	  Scale(sprite_data.Scale),
	  Radius(sprite_data.Radius * Scale)
{
	LoadImage(Scale);

	return;
}

IMGSprite::IMGSprite(const IMGSprite &obj)
	: Sprite(obj),
	  IMGPath(obj.IMGPath),
	  Scale(obj.Scale),
	  Radius(obj.Radius)
{
}

IMGSprite::IMGSprite(IMGSprite &&obj)
	: Sprite(std::move(obj)),
	  IMGPath(obj.IMGPath),
	  Scale(obj.Scale),
	  Radius(obj.Radius)
{
}

std::string IMGSprite::GetIMGPath() const
{
	return IMGPath;
}

void IMGSprite::SetIMGPath(std::string &img_path)
{
	IMGPath = img_path;
	LoadImage(Scale);

	return;
}

float IMGSprite::GetScale() const
{
	return Scale;
}

void IMGSprite::SetScale(float scale)
{
	SetRectSize(Rect.w * scale, Rect.h * scale);

	Scale = scale;

	return;
}

float IMGSprite::GetRadius() const
{
	return Radius;
}

void IMGSprite::SetRadius(float radius)
{
	Radius = radius;

	return;
}

bool IMGSprite::Collideswith(const IMGSprite &sprite) const
{
	float self_centerx = Rect.x + Rect.w / 2;
	float self_centery = Rect.y + Rect.h / 2;

	SDL_FRect sprite_rect = sprite.GetRect();
	float sprite_centerx = sprite_rect.x + sprite_rect.w / 2;
	float sprite_centery = sprite_rect.y + sprite_rect.h / 2;

	float difference_x = self_centerx - sprite_centerx;
	float difference_y = self_centery - sprite_centery;
	float distance = sqrtf(difference_x * difference_x +
						   difference_y * difference_y);

	if (distance <= Radius + sprite.GetRadius())
	{
		return true;
	}

	return false;
}