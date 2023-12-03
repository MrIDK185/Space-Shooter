#include "Text.hpp"

#include <SDL2/SDL_ttf.h>
#include <memory>
#include <sstream>
#include <vector>

std::unique_ptr<Uint16[]> to_uint16(std::wstring /*&*/ message)
{
	size_t array_size = message.length() + 1;
	std::unique_ptr<Uint16[]> unicodeMessage(new Uint16[array_size]);

	for (unsigned int idx = 0; idx < array_size; ++idx)
	{
		unicodeMessage[idx] = static_cast<Uint16>(message[idx]);
	}

	return unicodeMessage;
}

//*non-static(private)

void Text::SetText()
{
	std::unique_ptr<Uint16[]> unicodeMessage = to_uint16(Message);

	SDL_Surface *messageSurface = TTF_RenderUNICODE_Blended_Wrapped(Font, unicodeMessage.get(), fontColor, 0);
	Texture = SDL_CreateTextureFromSurface(destRenderer, messageSurface);

	int width, height;
	SDL_QueryTexture(Texture, nullptr, nullptr, &width, &height);
	SetRectSize(width, height);

	SDL_FreeSurface(messageSurface);
	messageSurface = nullptr;

	return;
}

void Text::SetTextCentered()
{
	if (Message.find(L"\n") == std::string::npos)
	{
		SetText();
		return;
	}

	std::vector<std::wstring> textLines;
	std::wstringstream stream(Message);
	std::wstring line;

	while (std::getline(stream, line))
	{
		textLines.push_back(line);
	}

	std::wstring longest_line = L"";
	for (auto &line : textLines)
	{
		if (line.length() > longest_line.length())
		{
			longest_line = line;
		}
	}

	int total_width = 0;
	for (auto character : longest_line)
	{
		int char_width = 0;
		TTF_GlyphMetrics32(Font, character, nullptr, nullptr, nullptr, nullptr, &char_width);
		total_width += char_width;
	}

	float lineHeight = static_cast<float>(TTF_FontLineSkip(Font));
	int total_height = lineHeight * textLines.size();
	float yPosition = 0;

	Texture = SDL_CreateTexture(destRenderer, SDL_PIXELFORMAT_RGBA32,
								SDL_TEXTUREACCESS_TARGET, total_width, total_height);
	SDL_SetTextureBlendMode(Texture, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget(destRenderer, Texture);

	for (auto &line : textLines)
	{
		std::unique_ptr<Uint16[]> unicodeMessage = to_uint16(line);
		SDL_Surface *lineSurface = TTF_RenderUNICODE_Blended(Font, unicodeMessage.get(), fontColor);
		if (lineSurface == nullptr)
		{
			break;
		}

		float lineWidth = static_cast<float>(lineSurface->w);
		float xPos = (total_width - lineWidth) / 2;

		SDL_Texture *lineTexture = SDL_CreateTextureFromSurface(destRenderer, lineSurface);
		SDL_FRect dstRect = {xPos, yPosition, lineWidth, lineHeight};

		SDL_RenderCopyF(destRenderer, lineTexture, nullptr, &dstRect);

		SDL_FreeSurface(lineSurface);
		SDL_DestroyTexture(lineTexture);

		yPosition += lineHeight;
	}

	SDL_SetRenderTarget(destRenderer, nullptr);

	int width,
		height;
	SDL_QueryTexture(Texture, nullptr, nullptr, &width, &height);
	SetRectSize(width, height);

	return;
}

//*non-static(public)

Text::Text(SDL_Renderer *renderer, std::wstring message, std::string font_path, unsigned int font_size,
		   SDL_Color font_color, bool centered)
	: Sprite(renderer),
	  Message(message),
	  Font(TTF_OpenFont(font_path.c_str(), font_size)),
	  fontColor(font_color)

{
	centered ? SetTextCentered() : SetText();

	return;
}

Text::~Text()
{
	TTF_CloseFont(Font);
	Font = nullptr;

	return;
}

std::wstring Text::GetMessage() const
{
	return Message;
}

void Text::SetMessage(std::wstring new_message, bool centered)
{
	Message = new_message;
	centered ? SetTextCentered() : SetText();

	return;
}

std::string Text::GetPath() const
{
	return Path;
}

void Text::SetPath(std::string path)
{
	Path = path;

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