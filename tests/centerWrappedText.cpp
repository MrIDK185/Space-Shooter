#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <sstream>
#include <vector>

SDL_Texture* renderWrappedText(const std::string& text, SDL_Renderer* renderer, TTF_Font* font, SDL_Color font_color) {
    std::vector<std::string> textLines;
    std::istringstream iss(text);
    std::string line;
    while (std::getline(iss, line)) {
        textLines.push_back(line);
    }

    std::string longest_line = "";
    for (const std::string& line : textLines)
    {
        if (line.length() > longest_line.length())
        {
            longest_line = line;
        }
    }

    int total_width = 0;
    for (const char character : longest_line)
    {
        int char_width;
        TTF_GlyphMetrics32(font, character, nullptr, nullptr, nullptr, nullptr, &char_width);
        total_width += char_width;
    }

    int lineHeight = TTF_FontLineSkip(font);
    int total_height = lineHeight * textLines.size();
    int yPosition = 0;

    SDL_Texture* combinedTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, total_width, total_height);
    SDL_SetRenderTarget(renderer, combinedTexture);

for (const std::string& line : textLines) {
        SDL_Surface* lineSurface = TTF_RenderText_Blended_Wrapped(font, line.c_str(), font_color, 0);
        if (lineSurface == nullptr) {
          break;
        }

        int lineTextWidth = lineSurface->w;
        int xPos = (total_width - lineTextWidth) / 2;

        SDL_Texture* lineTexture = SDL_CreateTextureFromSurface(renderer, lineSurface);
        SDL_Rect dstRect = {xPos, yPosition, lineTextWidth, lineHeight};

        SDL_RenderCopy(renderer, lineTexture, nullptr, &dstRect);

        SDL_FreeSurface(lineSurface);
        SDL_DestroyTexture(lineTexture);

        yPosition += lineHeight;
    }

    SDL_SetRenderTarget(renderer, nullptr);

    return combinedTexture;

}
