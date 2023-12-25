#include <random>

#include <Sprite/Asteroid.hpp>

//* non-static(public)

Asteroid::Asteroid(SpriteData sprite_data, AsteroidData asteroid_data)
	: IMGSprite(sprite_data),
	  Velocity(asteroid_data.Velocity)
{
}

Asteroid::Asteroid(const Asteroid &obj)
	: IMGSprite(obj),
	  Angle(obj.Angle),
	  Velocity(obj.Velocity)
{
}

Asteroid::Asteroid(Asteroid &&obj)
	: IMGSprite(std::move(obj)),
	  Angle(obj.Angle),
	  Velocity(obj.Velocity)
{
}

void Asteroid::Move(int screen_width, int screen_height, float delta_time_seconds)
{
	Rect.x += std::sin(Angle * M_PI / 180) * Velocity * delta_time_seconds;
	Rect.y -= std::cos(Angle * M_PI / 180) * Velocity * delta_time_seconds;

	if (Rect.x <= -Rect.w)
	{
		Rect.x = screen_width;
	}
	else if (Rect.x >= screen_width)
	{
		Rect.x = -Rect.w;
	}
	if (Rect.y <= -Rect.h)
	{
		Rect.y = screen_height;
	}
	else if (Rect.y >= screen_height)
	{
		Rect.y = -Rect.h;
	}

	return;
}

void Asteroid::Randomize(int screen_width, int screen_height)
{
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution<> random_angle(0, 359);

	Angle = static_cast<unsigned int>(random_angle(gen));

	std::uniform_int_distribution<> random_x_pos(0, screen_width - Rect.w);
	std::uniform_int_distribution<> random_y_pos(0, screen_height - Rect.h);

	SetRectPos(random_x_pos(gen), random_y_pos(gen));

	return;
}

void Asteroid::Render() const
{
	SDL_RenderCopyExF(destRenderer, Texture.get(), nullptr, &Rect, Angle, nullptr, SDL_FLIP_NONE);

	return;
}