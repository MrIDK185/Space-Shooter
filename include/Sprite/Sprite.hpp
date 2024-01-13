#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <optional>

#include <SDL2/SDL.h>

#include "Sprite/Alignments.hpp"
#include "uniquePointerDeleter.hpp"

class Sprite
{
protected:
	//* static

	static inline void surfaceDestructor(SDL_Surface *surface) { SDL_FreeSurface(surface); }

	static inline void textureDestructor(SDL_Texture *texture) { SDL_DestroyTexture(texture); }

	//* non-static

	std::optional<SDL_Renderer *> destRenderer = std::nullopt;

	unique_ptr_deleter<SDL_Surface> Surface = {nullptr, surfaceDestructor};
	unique_ptr_deleter<SDL_Texture> Texture = {nullptr, textureDestructor};

	SDL_FRect Rect = {0, 0, 0, 0};

public:
	//* non-static

	explicit Sprite(SDL_Renderer *renderer);

	explicit Sprite(const Sprite &obj);

	explicit Sprite(Sprite &&obj);

	explicit Sprite() = default;

	virtual ~Sprite();

	std::optional<SDL_Renderer *const> GetRenderer() const;

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