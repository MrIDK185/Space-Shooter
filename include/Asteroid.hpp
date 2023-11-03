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

	Asteroid(SDL_Renderer *renderer, std::string path, float scale, float radius, float velocity, int screen_width, int screen_height);

	~Asteroid() override = default;

	void Move(int screen_width, int screen_height, float delta_time_seconds);

	void Randomize(int screen_width, int screen_height);

	void Render() const override;
};

#endif //! ASTEROID_HPP