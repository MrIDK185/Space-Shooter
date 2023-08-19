#ifndef TEXT_HPP
#define TEXT_HPP

#include "Sprite.hpp"

typedef struct _TTF_Font TTF_Font;

class Text : public Sprite
{
private:
	//*non-static

	const wchar_t *Message = nullptr;
	TTF_Font *Font;
	SDL_Color fontColor;

	void SetText();

public:
	//*non-static

	Text(SDL_Renderer *renderer, const wchar_t *message, const char *path, unsigned int size, SDL_Color color);

	~Text() override;

	const wchar_t *GetMessage();

	void SetMessage(const wchar_t *new_message);

	TTF_Font *GetFont();

	void SetFont(TTF_Font *new_font);

	SDL_Color GetFontColor();

	void SetFontColor(SDL_Color new_font_color);

	void Render() override;
};

#endif //! TEXT_HPP