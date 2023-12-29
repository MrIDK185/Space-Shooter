#ifndef IMG_SPRITE_HPP
#define IMG_SPRITE_HPP

#include "Sprite/Sprite.hpp"
#include "Sprite/SpriteData.hpp"

class IMGSprite : public Sprite
{
protected:
	//* non-static

	std::string IMGPath;

	float Scale;
	unsigned int Radius;

	void LoadImage(float scale = 1);

public:
	//* non-static

	IMGSprite(SpriteData sprite_data);

	explicit IMGSprite(const IMGSprite &obj);

	explicit IMGSprite(IMGSprite &&obj);

	~IMGSprite() override = default;

	std::string GetIMGPath() const;

	void SetIMGPath(std::string &img_path);

	float GetScale() const;

	void SetScale(float scale);

	float GetRadius() const;

	void SetRadius(float radius);

	bool Collideswith(const IMGSprite &sprite) const;
};

#endif //! IMG_SPRITE_HPP