#include "Sprite/Sprite.hpp"

//* non-static(public)

Sprite::Sprite(SDL_Renderer *renderer)
	: destRenderer(renderer)
{
}

Sprite::Sprite(const Sprite &obj)
	: destRenderer(obj.destRenderer),
	  Surface(new SDL_Surface(*obj.Surface), surfaceDestructor),
	  Rect(obj.Rect)
{
	SetTexture(SDL_CreateTextureFromSurface(*destRenderer, Surface.get()));

	return;
}

Sprite::Sprite(Sprite &&obj)
	: destRenderer(obj.destRenderer),
	  Surface(std::move(obj.Surface)),
	  Texture(std::move(obj.Texture)),
	  Rect(obj.Rect)
{
}

Sprite::~Sprite()
{
	Surface = nullptr;
	Texture = nullptr;

	return;
}

std::optional<SDL_Renderer *const> Sprite::GetRenderer() const
{
	return destRenderer;
}

unique_ptr_deleter<SDL_Surface> Sprite::GetSurface()
{
	return std::move(Surface);
}

void Sprite::SetSurface(SDL_Surface *surface)
{
	Surface.reset(surface);

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
	SDL_RenderCopyF(*destRenderer, Texture.get(), nullptr, &Rect);

	return;
}