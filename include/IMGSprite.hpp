#ifndef IMG_SPRITE_HPP
#define IMG_SPRITE_HPP

#include "Sprite.hpp"

#include <string>
#include <memory>

class IMGSprite : public Sprite
{
protected:
	//*non-static

	std::string IMGPath;

	float
		Scale,
		Radius;

	void LoadImage();

public:
	//*non-static

	IMGSprite(SDL_Renderer *renderer, std::string path, float scale, float radius);

	~IMGSprite() override;

	std::string GetIMGPath() const;

	void SetIMGPath(std::string img_path);

	float GetScale() const;

	void SetScale(float factor);

	float GetRadius() const;

	void SetRadius(float radius);

	bool Collideswith(std::shared_ptr<IMGSprite> sprite) const;
};

#endif //! IMG_SPRITE_HPP