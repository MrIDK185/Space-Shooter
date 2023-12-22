#include <sstream>
#include <vector>

#include "Text.hpp"

std::unique_ptr<Uint16[]> to_uint16(std::wstring message)
{
	size_t array_size = message.length() + 1;
	std::unique_ptr<Uint16[]> unicodeMessage(new Uint16[array_size]);

	for (unsigned int idx = 0; idx < array_size; ++idx)
	{
		unicodeMessage[idx] = static_cast<Uint16>(message[idx]);
	}

	return unicodeMessage;
}

//* non-static(private)

void Text::LoadFont()
{
	SetFont(TTF_OpenFont(fontPath.c_str(), fontSize));

	return;
}

void Text::SetText()
{
	std::unique_ptr<Uint16[]> unicodeMessage = to_uint16(Message);

	SetSurface(TTF_RenderUNICODE_Blended_Wrapped(Font.get(), unicodeMessage.get(), fontColor, 0));
	SetTexture(SDL_CreateTextureFromSurface(destRenderer, Surface.get()));

	int width, height;
	SDL_QueryTexture(Texture.get(), nullptr, nullptr, &width, &height);
	SetRectSize(width, height);

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
		TTF_GlyphMetrics32(Font.get(), character, nullptr, nullptr, nullptr, nullptr, &char_width);
		total_width += char_width;
	}

	float lineHeight = static_cast<float>(TTF_FontLineSkip(Font.get()));
	int total_height = lineHeight * textLines.size();
	float yPosition = 0;

	SetTexture(SDL_CreateTexture(destRenderer, SDL_PIXELFORMAT_RGBA32,
								 SDL_TEXTUREACCESS_TARGET, total_width, total_height));
	SDL_SetTextureBlendMode(Texture.get(), SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget(destRenderer, Texture.get());

	for (auto &line : textLines)
	{
		std::unique_ptr<Uint16[]> unicodeMessage = to_uint16(line);
		unique_ptr_deleter<SDL_Surface> lineSurface = {TTF_RenderUNICODE_Blended(Font.get(), unicodeMessage.get(), fontColor),
													   surfaceDestructor};
		if (lineSurface == nullptr)
		{
			break;
		}

		float lineWidth = static_cast<float>(lineSurface->w);
		float xPos = (total_width - lineWidth) / 2;

		unique_ptr_deleter<SDL_Texture> lineTexture = {SDL_CreateTextureFromSurface(destRenderer, lineSurface.get()),
													   textureDestructor};
		SDL_FRect dstRect = {xPos, yPosition, lineWidth, lineHeight};

		SDL_RenderCopyF(destRenderer, lineTexture.get(), nullptr, &dstRect);

		yPosition += lineHeight;
	}

	SDL_SetRenderTarget(destRenderer, nullptr);

	int width, height;
	SDL_QueryTexture(Texture.get(), nullptr, nullptr, &width, &height);
	SetRectSize(width, height);

	return;
}

//* non-static(public)

Text::Text(TextData text_data, bool centered)
	: Sprite(text_data.destRenderer),
	  Message(text_data.Message),
	  fontPath(text_data.fontPath),
	  fontColor(text_data.fontColor),
	  fontSize(text_data.fontSize)
{
	LoadFont();
	centered ? SetTextCentered() : SetText();

	return;
}

Text::Text(const Text &obj)
	: Sprite(obj),
	  Message(obj.Message),
	  Font(nullptr, fontDestructor),
	  fontPath(obj.fontPath),
	  fontColor(obj.fontColor),
	  fontSize(obj.fontSize)
{
	LoadFont();

	return;
}

Text::Text(Text &&obj)
	: Sprite(std::move(obj)),
	  Message(obj.Message),
	  Font(std::move(obj.Font)),
	  fontPath(obj.fontPath),
	  fontColor(obj.fontColor),
	  fontSize(obj.fontSize)
{
}

Text::~Text()
{
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
	SDL_DestroyTexture(Texture.get());
	centered ? SetTextCentered() : SetText();

	return;
}

unique_ptr_deleter<TTF_Font> Text::GetFont()
{
	return std::move(Font);
}

void Text::SetFont(TTF_Font *new_font)
{
	Font.reset(new_font);

	return;
}

std::string Text::GetFontPath() const
{
	return fontPath;
}

void Text::SetFontPath(std::string path)
{
	fontPath = path;

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

unsigned int Text::GetFontSize()
{
	return fontSize;
}

void Text::SetFontSize(unsigned int font_size)
{
	fontSize = font_size;

	return;
}

void Text::Render() const
{
	SDL_RenderCopyF(destRenderer, Texture.get(), nullptr, &Rect);

	return;
}