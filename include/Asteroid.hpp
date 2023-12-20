#ifndef ASTEROID_HPP
#define ASTEROID_HPP

#include "IMGSprite.hpp"

typedef struct
{
	float Velocity;
} AsteroidData;

class Asteroid : public IMGSprite
{
private:
	//* non-static

	unsigned int Angle;

	float Velocity;

public:
	//* non-static

	Asteroid(SpriteData sprite_data, AsteroidData asteroid_data);

	explicit Asteroid(Asteroid &&obj);

	~Asteroid() override = default;

	void Move(int screen_width, int screen_height, float delta_time_seconds);

	void Randomize(int screen_width, int screen_height);

	void Render() const override;
};

#endif //! ASTEROID_HPP