#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <SDL2/SDL.h>

class Sprite
{
protected:
	//*non-static

	SDL_Renderer *destRenderer;
	SDL_Texture *Texture;
	SDL_FRect Rect = {0, 0, 0, 0};

	void Init();

public:
	//*non-static

	explicit Sprite(SDL_Renderer *renderer);

	virtual ~Sprite();

	SDL_Renderer *GetRenderer();

	void SetRenderer(SDL_Renderer *renderer);

	SDL_Texture *GetTexture();

	void SetTexture(SDL_Texture *texture);

	SDL_FRect GetRect() const;

	void SetRectSize(float width, float height);

	void SetRectPos(float pos_x, float pos_y);

	virtual void Render();
};

#endif //! SPRITE_HPP