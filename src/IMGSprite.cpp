#include "IMGSprite.hpp"

#include <SDL2/SDL_image.h>

//* non-static(protected)

void IMGSprite::LoadImage()
{
	SDL_Surface *loadedSurface = IMG_Load(IMGPath.c_str());
	Texture = SDL_CreateTextureFromSurface(destRenderer, loadedSurface);

	int width, height;
	SDL_QueryTexture(Texture, nullptr, nullptr, &width, &height);
	SetRectSize(width, height);

	SDL_FreeSurface(loadedSurface);
	loadedSurface = nullptr;

	return;
}

//* non-static(public)

IMGSprite::IMGSprite(SDL_Renderer *renderer, std::string path, float scale, float radius)
	: Sprite(renderer),
	  IMGPath(path),
	  Scale(scale),
	  Radius(radius * scale)
{
	LoadImage();
	SetScale(scale);

	return;
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