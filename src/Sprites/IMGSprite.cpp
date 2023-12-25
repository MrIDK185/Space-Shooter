#include "IMGSprite.hpp"

#include <SDL2/SDL_image.h>

//* non-static(protected)

void IMGSprite::LoadImage()
{
	SetSurface(IMG_Load(IMGPath.c_str()));
	SetTexture(SDL_CreateTextureFromSurface(destRenderer, Surface.get()));

	int width, height;
	SDL_QueryTexture(Texture.get(), nullptr, nullptr, &width, &height);
	SetRectSize(width, height);

	return;
}

//* non-static(public)

IMGSprite::IMGSprite(SpriteData sprite_data)
	: Sprite(sprite_data.destRenderer),
	  IMGPath(sprite_data.IMGPath),
	  Scale(sprite_data.Scale),
	  Radius(sprite_data.Radius * Scale)
{
	LoadImage();
	SetScale(Scale);

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

void IMGSprite::SetIMGPath(std::string img_path)
{
	IMGPath = img_path;
	LoadImage();

	return;
}

float IMGSprite::GetScale() const
{
	return Scale;
}

void IMGSprite::SetScale(float factor)
{
	SetRectSize(Rect.w * factor, Rect.h * factor);

	Scale = factor;

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
	float self_radius = Radius;
	SDL_FRect self_rect = Rect;
	int self_centerx = self_rect.x + self_rect.w / 2;
	int self_centery = self_rect.y + self_rect.h / 2;

	float sprite_radius = sprite.GetRadius();
	SDL_FRect sprite_rect = sprite.GetRect();
	int sprite_centerx = sprite_rect.x + sprite_rect.w / 2;
	int sprite_centery = sprite_rect.y + sprite_rect.h / 2;

	int difference_x = self_centerx - sprite_centerx;
	int difference_y = self_centery - sprite_centery;
	float distance = sqrtf(difference_x * difference_x + difference_y * difference_y);

	if (distance <= self_radius + sprite_radius)
	{
		return true;
	}

	return false;
}