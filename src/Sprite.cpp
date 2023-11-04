#include "Sprite.hpp"

//*non-static(public)

Sprite::Sprite(SDL_Renderer *renderer) : destRenderer(renderer)
{
}

Sprite::~Sprite()
{
	SDL_DestroyTexture(Texture);
	Texture = nullptr;

	destRenderer = nullptr;

	return;
}

SDL_Renderer *Sprite::GetRenderer() const
{
	return destRenderer;
}

void Sprite::SetRenderer(SDL_Renderer *renderer)
{
	destRenderer = renderer;

	return;
}

SDL_Texture *Sprite::GetTexture() const
{
	return Texture;
}

void Sprite::SetTexture(SDL_Texture *texture)
{
	Texture = texture;

	return;
}

SDL_FRect Sprite::GetRect() const
{
	return Rect;
}

void Sprite::SetRectSize(float width, float height)
{
	Rect.w = width;
	Rect.h = height;

	return;
}

void Sprite::SetRectPos(float pos_x, float pos_y)
{
	Rect.x = pos_x - Rect.w / 2;
	Rect.y = pos_y - Rect.h / 2;

	return;
}

void Sprite::Render() const
{
	SDL_RenderCopyF(destRenderer, Texture, nullptr, &Rect);

	return;
}