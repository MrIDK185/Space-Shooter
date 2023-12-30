#ifndef SPRITE_DATA_HPP
#define SPRITE_DATA_HPP

#include <string>

#include <SDL2/SDL.h>

struct SpriteData
{
	SDL_Renderer *destRenderer;

	std::string IMGPath;

	float Scale;

	unsigned int Radius;
};

#endif //! SPRITE_DATA_HPP