#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <memory>

#include <SDL2/SDL.h>
#include "uniquePointerDeleter.hpp"

typedef enum
{
	NW,
	NE,
	SE,
	SW,
	CENTER
} Alignments;

class Sprite
{
protected:
	//* static

	static inline void surfaceDestructor(SDL_Surface *surface) { SDL_FreeSurface(surface); }
	static inline void textureDestructor(SDL_Texture *texture) { SDL_DestroyTexture(texture); }

	//* non-static

	SDL_Renderer *destRenderer;
	unique_ptr_deleter<SDL_Surface> Surface = {nullptr, surfaceDestructor};
	unique_ptr_deleter<SDL_Texture> Texture = {nullptr, textureDestructor};
	SDL_FRect Rect = {0, 0, 0, 0};

public:
	//* non-static

	explicit Sprite(SDL_Renderer *renderer);

	explicit Sprite(const Sprite &obj);

	explicit Sprite(Sprite &&obj);

	virtual ~Sprite();

	SDL_Renderer *GetRenderer() const;

	void SetRenderer(SDL_Renderer *renderer);

	unique_ptr_deleter<SDL_Surface> GetSurface();

	void SetSurface(SDL_Surface *surface);

	unique_ptr_deleter<SDL_Texture> GetTexture();

	void SetTexture(SDL_Texture *texture);

	SDL_FRect GetRect() const;

	void SetRectSize(float width, float height);

	virtual void SetRectPos(float pos_x, float pos_y, Alignments align = CENTER);

	virtual void Render() const;
};

#endif //! SPRITE_HPP