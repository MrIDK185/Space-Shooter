#ifndef ASTEROID_HPP
#define ASTEROID_HPP

#include "IMGSprite.hpp"
#include "Configuration.hpp"

class Asteroid : public IMGSprite
{
private:
	//* non-static

	unsigned int Angle;

	float Velocity;

	Configuration &Config;

public:
	//* non-static

	Asteroid(SDL_Renderer *renderer, const char *path, float scale, float radius, float velocity, Configuration &config);

	~Asteroid() override;

	void Move();

	void Render() override;
};

#endif //! ASTEROID_HPP