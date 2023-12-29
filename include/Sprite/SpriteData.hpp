#ifndef SPRITE_DATA_HPP
#define SPRITE_DATA_HPP

#include <string>

#include <SDL2/SDL.h>

typedef struct
{
	SDL_Renderer *destRenderer;

	std::string IMGPath;

	float Scale;

	unsigned int Radius;
} SpriteData;

#endif //! SPRITE_DATA_HPP