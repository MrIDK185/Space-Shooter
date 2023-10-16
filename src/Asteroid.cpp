#include <Asteroid.hpp>

#include <random>

Asteroid::Asteroid(SDL_Renderer *renderer, std::string path, float scale, float radius, float velocity,
				   int screen_width, int screen_height)
	: IMGSprite::IMGSprite(renderer, path, scale, radius),
	  Velocity(velocity)
{
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution<> random_angle(0, 359);

	Angle = static_cast<unsigned int>(random_angle(gen));

	std::uniform_int_distribution<> random_x_pos(0, screen_width - Rect.w);
	std::uniform_int_distribution<> random_y_pos(0, screen_height - Rect.h);

	SetRectPos(random_x_pos(gen), random_y_pos(gen));
}

Asteroid::~Asteroid()
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

void Asteroid::Render() const
{
	SDL_RenderCopyExF(destRenderer, Texture, nullptr, &Rect, Angle, nullptr, SDL_FLIP_NONE);
}