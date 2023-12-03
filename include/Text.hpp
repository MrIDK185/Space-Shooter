#ifndef TEXT_HPP
#define TEXT_HPP

#include "Sprite.hpp"

#include <string>

typedef struct _TTF_Font TTF_Font;

class Text : public Sprite
{
private:
	//*non-static

	std::wstring Message;
	std::string Path;

	TTF_Font *Font;
	SDL_Color fontColor;

	void SetText();
	void SetTextCentered();

public:
	//*non-static

	Text(SDL_Renderer *renderer, std::wstring message, std::string font_path, unsigned int font_size,
		 SDL_Color font_color, bool centered = true);

	~Text() override;

	std::wstring GetMessage() const;

	void SetMessage(std::wstring new_message, bool centered = true);

	std::string GetPath() const;

	void SetPath(std::string path);

	TTF_Font *GetFont() const;

	void SetFont(TTF_Font *new_font);

	SDL_Color GetFontColor() const;

	void SetFontColor(SDL_Color new_font_color);

	void Render() const override;
};

#endif //! TEXT_HPP