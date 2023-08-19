#include "Text.hpp"

#include <SDL2/SDL_ttf.h>

//*non-static(private)

void Text::SetText()
{
	size_t size = wcslen(Message);
	Uint16 uc_arr[size + 1];

	for (unsigned int idx = 0; idx < size; idx++)
	{
		uc_arr[idx] = static_cast<Uint16>(Message[idx]);
	}
	uc_arr[size] = static_cast<Uint16>(L'\0');

	const Uint16 *unicodeMessage = static_cast<const Uint16 *>(uc_arr);

	SDL_Surface *messageSurface = TTF_RenderUNICODE_Blended_Wrapped(Font, unicodeMessage, fontColor, 0);
	Texture = SDL_CreateTextureFromSurface(destRenderer, messageSurface);

	int width, height;
	SDL_QueryTexture(this->GetTexture(), nullptr, nullptr, &width, &height);
	this->SetRectSize(width, height);

	SDL_FreeSurface(messageSurface);
	messageSurface = nullptr;

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