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
	SetRectSize(width, height);

	SDL_FreeSurface(messageSurface);
	messageSurface = nullptr;

	delete[] unicodeMessage;
	unicodeMessage = nullptr;

	return;
}

//*non-static(public)

Text::Text(SDL_Renderer *renderer, const wchar_t *message, const char *font_path, unsigned int font_size,
		   SDL_Color font_color)
	: Sprite(renderer),
	  Message(message),
	  Font(TTF_OpenFont(font_path, font_size)),
	  fontColor(font_color)

{
	SetText();

	return;
}

Text::~Text()
{
	TTF_CloseFont(Font);
	Font = nullptr;

	Message = nullptr;

	return;
}

const wchar_t *Text::GetMessage() const
{
	return Message;
}

void Text::SetMessage(const wchar_t *new_message)
{
	Message = new_message;
	SetText();

	return;
}

TTF_Font *Text::GetFont() const
{
	return Font;
}

void Text::SetFont(TTF_Font *new_font)
{
	Font = new_font;

	return;
}

SDL_Color Text::GetFontColor() const
{
	return fontColor;
}

void Text::SetFontColor(SDL_Color new_font_color)
{
	fontColor = new_font_color;

	return;
}

void Text::Render() const
{
	SDL_RenderCopyF(destRenderer, Texture, nullptr, &Rect);

	return;
}