#ifndef TEXT_DATA_HPP
#define TEXT_DATA_HPP

#include <string>

#include <SDL2/SDL.h>

typedef struct
{
	SDL_Renderer *destRenderer;

	std::wstring Message;

	std::string fontPath;

	SDL_Color fontColor;

	unsigned int fontSize;
} TextData;

#endif // !TEXT_DATA_HPP