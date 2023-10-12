#include "Text.hpp"

#include <SDL2/SDL_ttf.h>

//*non-static(private)

void Text::SetText()
{
	size_t array_size = wcslen(Message) + 1;
	Uint16 *unicodeMessage = new Uint16[array_size];

	for (unsigned int idx = 0; idx < array_size; idx++)
	{
		unicodeMessage[idx] = static_cast<Uint16>(Message[idx]);
	}

	SDL_Surface *messageSurface = TTF_RenderUNICODE_Blended_Wrapped(Font, unicodeMessage, fontColor, 0);
	Texture = SDL_CreateTextureFromSurface(destRenderer, messageSurface);

	int width, height;
	SDL_QueryTexture(Texture, nullptr, nullptr, &width, &height);
	this->SetRectSize(width, height);

	SDL_FreeSurface(messageSurface);
	messageSurface = nullptr;

	delete[] unicodeMessage;
	unicodeMessage = nullptr;

	return;
}

//*non-static(public)

Text::Text(SDL_Renderer *renderer, const wchar_t *message, const char *path, unsigned int size, SDL_Color color) : Sprite(renderer),
																												   Message(message),
																												   Font(TTF_OpenFont(path, size)),
																												   fontColor(color)

{
	this->SetText();

	return;
}

Text::~Text()
{
	TTF_CloseFont(Font);
	Font = nullptr;

	Message = nullptr;

	return;
}

const wchar_t *Text::GetMessage()
{
	return Message;
}

void Text::SetMessage(const wchar_t *new_message)
{
	Message = new_message;
	this->SetText();

	return;
}

TTF_Font *Text::GetFont()
{
	return Font;
}

void Text::SetFont(TTF_Font *new_font)
{
	Font = new_font;

	return;
}

SDL_Color Text::GetFontColor()
{
	return fontColor;
}

void Text::SetFontColor(SDL_Color new_font_color)
{
	fontColor = new_font_color;

	return;
}

void Text::Render()
{
	SDL_FRect destrect = Rect;
	SDL_RenderCopyF(destRenderer, Texture, nullptr, &destrect);

	return;
}