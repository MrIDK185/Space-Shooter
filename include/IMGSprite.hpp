#ifndef IMG_SPRITE_HPP
#define IMG_SPRITE_HPP

#include "Sprite.hpp"

#include <memory>

class IMGSprite : public Sprite
{
protected:
	//*non-static

	const char *IMGPath = nullptr;

	float
		Scale,
		Radius;

	void LoadImage();

public:
	//*non-static

	IMGSprite(SDL_Renderer *renderer, const char *path, float scale, float radius);

	~IMGSprite() override;

	float GetScale() const;

	void SetScale(float factor);

	float GetRadius() const;

	void SetRadius(float radius);

	bool Collideswith(std::shared_ptr<IMGSprite> sprite) const;
};

#endif //! IMG_SPRITE_HPP