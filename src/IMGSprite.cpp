#include "IMGSprite.hpp"

#include <SDL2/SDL_image.h>

//*non-static(public)

IMGSprite::IMGSprite(SDL_Renderer *renderer, const char *path, float scale, float radius) : Sprite(renderer),
																							IMGPath(path),
																							Scale(scale),
																							Radius(radius * scale)
{
	this->SetIMGTexture();
	this->SetScale(scale);

	return;
}

IMGSprite::~IMGSprite()
{
	IMGPath = nullptr;
}

float IMGSprite::GetScale() const
{
	return Scale;
}

void IMGSprite::SetScale(float factor)
{
	this->SetRectSize(Rect.w * factor, Rect.h * factor);

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

void IMGSprite::SetIMGTexture()
{
	SDL_Surface *loadedSurface = IMG_Load(IMGPath);
	Texture = SDL_CreateTextureFromSurface(destRenderer, loadedSurface);

	int width, height;
	SDL_QueryTexture(Texture, nullptr, nullptr, &width, &height);
	this->SetRectSize(width, height);

	SDL_FreeSurface(loadedSurface);
	loadedSurface = nullptr;

	return;
}

bool IMGSprite::Collideswith(std::shared_ptr<IMGSprite> sprite) const
{
	float self_radius = Radius;
	SDL_FRect self_rect = Rect;
	int self_centerx = self_rect.x + self_rect.w / 2;
	int self_centery = self_rect.y + self_rect.h / 2;

	float sprite_radius = sprite->GetRadius();
	SDL_FRect sprite_rect = sprite->GetRect();
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