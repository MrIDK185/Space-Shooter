#include "Sprite.hpp"

//*non-static(protected)

void Sprite::Init()
{
	SDL_Surface *loadedSurface = SDL_CreateRGBSurface(0, 0, 0, 0, 0, 0, 0, 0);
	Texture = SDL_CreateTextureFromSurface(destRenderer, loadedSurface);

	SDL_FreeSurface(loadedSurface);
	loadedSurface = nullptr;

	return;
}

//*non-static(public)

Sprite::Sprite(SDL_Renderer *renderer) : destRenderer(renderer)
{
	this->Init();

	return;
}

Sprite::~Sprite()
{
	SDL_DestroyTexture(Texture);
	Texture = nullptr;

	destRenderer = nullptr;

	return;
}

SDL_Renderer *Sprite::GetRenderer()
{
	return destRenderer;
}

void Sprite::SetRenderer(SDL_Renderer *renderer)
{
	destRenderer = renderer;

	return;
}

SDL_Texture *Sprite::GetTexture()
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
	Rect.x = pos_x;
	Rect.y = pos_y;

	return;
}

void Sprite::Render()
{
	SDL_RenderCopyF(destRenderer, Texture, nullptr, &Rect);

	return;
}