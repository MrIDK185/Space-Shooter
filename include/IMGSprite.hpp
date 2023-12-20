#ifndef IMG_SPRITE_HPP
#define IMG_SPRITE_HPP

#include <string>

#include "Sprite.hpp"

typedef struct
{
	SDL_Renderer *destRenderer;

	std::string IMGPath;

	float
		Scale,
		Radius;
} SpriteData;

class IMGSprite : public Sprite
{
protected:
	//* non-static

	std::string IMGPath;

	float
		Scale,
		Radius;

	void LoadImage();

public:
	//* non-static

	IMGSprite(SpriteData sprite_data);

	explicit IMGSprite(IMGSprite &&obj);

	~IMGSprite() override = default;

	std::string GetIMGPath() const;

	void SetIMGPath(std::string img_path);

	float GetScale() const;

	void SetScale(float factor);

	float GetRadius() const;

	void SetRadius(float radius);

	bool Collideswith(const IMGSprite &sprite) const;
};

#endif //! IMG_SPRITE_HPP