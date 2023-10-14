#ifndef ASTEROID_HPP
#define ASTEROID_HPP

#include "IMGSprite.hpp"

class Asteroid : public IMGSprite
{
private:
	//* non-static

	unsigned int Angle;

	float Velocity;

public:
	//* non-static

	Asteroid(SDL_Renderer *renderer, const char *path, float scale, float radius, float velocity, int screen_width, int screen_height);

	~Asteroid() override;

	void Move(int screen_width, int screen_height, float delta_time_seconds);

	void Render() override;
};

#endif //! ASTEROID_HPP