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

	SDL_Color fontColor;

	unsigned int fontSize;
} TextData;

class Text : public Sprite
{
private:
	//* static

	static inline void fontDestructor(TTF_Font *font) { TTF_CloseFont(font); }

	//*non-static

	std::wstring Message;

	unique_ptr_deleter<TTF_Font> Font = {nullptr, fontDestructor};
	std::string fontPath;
	SDL_Color fontColor;
	unsigned int fontSize;

	void LoadFont();
	void SetText();
	void SetTextCentered();

public:
	//*non-static

	Text(TextData text_data, bool centered = true);

	explicit Text(const Text &obj);

	explicit Text(Text &&obj);

	~Text() override;

	std::wstring GetMessage() const;

	void SetMessage(std::wstring new_message, bool centered = true);

	unique_ptr_deleter<TTF_Font> GetFont();

	void SetFont(TTF_Font *new_font);

	std::string GetFontPath() const;

	void SetFontPath(std::string path);

	SDL_Color GetFontColor() const;

	void SetFontColor(SDL_Color new_font_color);

	unsigned int GetFontSize();

	void SetFontSize(unsigned int font_size);

	void Render() const override;
};

#endif //! TEXT_HPP