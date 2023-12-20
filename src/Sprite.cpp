#include "Sprite.hpp"
#include <iostream>

//*non-static(public)

Sprite::Sprite(SDL_Renderer *renderer) : destRenderer(renderer)
{
}

Sprite::~Sprite()
{
	// SDL_DestroyTexture(Texture);
	Texture = nullptr;

	destRenderer = nullptr;
	std::cout << "Sprite destructor called\n";

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

unique_ptr_deleter<SDL_Texture> Sprite::GetTexture()
{
	return std::move(Texture);
}

void Sprite::SetTexture(SDL_Texture *texture)
{
	Texture.reset(texture);

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

void Sprite::SetRectPos(float pos_x, float pos_y, Alignments align)
{
	switch (align)
	{
	case NW:
		break;
	case NE:
		pos_x -= Rect.w;
		break;
	case SE:
		pos_x -= Rect.w;
		pos_y -= Rect.h;
		break;
	case SW:
		pos_y -= Rect.h;
		break;
	case CENTER:
		pos_x -= Rect.w / 2;
		pos_y -= Rect.h / 2;
		break;
	default:
		break;
	}

	Rect.x = pos_x;
	Rect.y = pos_y;

	return;
}

void Sprite::Render() const
{
	SDL_RenderCopyF(destRenderer, Texture.get(), nullptr, &Rect);

	return;
}