#ifndef ASTEROID_HPP
#define ASTEROID_HPP

#include "IMGSprite.hpp"

class Asteroid : public IMGSprite
{
private:
	//* non-static

	unsigned int Angle;

public:
	//* non-static

	Asteroid(SDL_Renderer *renderer, const char *path, float scale, float radius);

	~Asteroid() override;

	void Render() override;
};

#endif //! ASTEROID_HPP