#ifndef TEXT_HPP
#define TEXT_HPP

#include <string>

#include <SDL2/SDL_ttf.h>

#include "Sprite.hpp"

typedef struct
{
	SDL_Renderer *destRenderer;

	std::wstring Message;

	std::string fontPath;

	unsigned int fontSize;

	SDL_Color fontColor;
} TextData;

class Text : public Sprite
{
private:
	//* static

	static inline void fontDestructor(TTF_Font *font) { TTF_CloseFont(font); }

	//*non-static

	std::wstring Message;
	std::string Path;

	unique_ptr_deleter<TTF_Font> Font = {nullptr, fontDestructor};
	SDL_Color fontColor;
	unsigned int fontSize;

	void LoadFont();
	void SetText();
	void SetTextCentered();

public:
	//*non-static

	Text(TextData text_data, bool centered = true);

	explicit Text(Text &&obj);

	~Text() override;

	std::wstring GetMessage() const;

	void SetMessage(std::wstring new_message, bool centered = true);

	std::string GetPath() const;

	void SetPath(std::string path);

	unique_ptr_deleter<TTF_Font> GetFont();

	void SetFont(TTF_Font *new_font);

	SDL_Color GetFontColor() const;

	void SetFontColor(SDL_Color new_font_color);

	unsigned int GetFontSize();

	void SetFontSize(unsigned int font_size);

	void Render() const override;
};

#endif //! TEXT_HPP