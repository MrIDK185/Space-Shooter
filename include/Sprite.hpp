#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <memory>
#include <iostream>

#include <SDL2/SDL.h>

template <typename T>
using unique_ptr_deleter = std::unique_ptr<T, void (*)(T *)>;

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
private:
	static inline void textureDestructor(SDL_Texture *texture)
	{
		std::cout << "Destroyed Texture\n";
		SDL_DestroyTexture(texture);
	}

protected:
	//*non-static

	SDL_Renderer *destRenderer;
	// SDL_Texture *Texture = nullptr;
	unique_ptr_deleter<SDL_Texture> Texture = {nullptr, textureDestructor};
	SDL_FRect Rect = {0, 0, 0, 0};

public:
	//*non-static

	explicit Sprite(SDL_Renderer *renderer);

	virtual ~Sprite();

	SDL_Renderer *GetRenderer() const;

	void SetRenderer(SDL_Renderer *renderer);

	unique_ptr_deleter<SDL_Texture> GetTexture();

	void SetTexture(SDL_Texture *texture);

	SDL_FRect GetRect() const;

	void SetRectSize(float width, float height);

	virtual void SetRectPos(float pos_x, float pos_y, Alignments align = CENTER);

	virtual void Render() const;
};

#endif //! SPRITE_HPP