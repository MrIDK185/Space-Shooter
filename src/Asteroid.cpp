#include <Asteroid.hpp>

#include <random>

Asteroid::Asteroid(SDL_Renderer *renderer, const char *path, float scale, float radius, float velocity, int screen_width, int screen_height) : IMGSprite::IMGSprite(renderer, path, scale, radius),
																																			   Velocity(velocity)
{
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution<> random_angle(0, 359);

	Angle = static_cast<unsigned int>(random_angle(gen));

	std::uniform_int_distribution<> random_x_pos(0, screen_width - Rect.w);
	std::uniform_int_distribution<> random_y_pos(0, screen_height - Rect.h);

	this->SetRectPos(random_x_pos(gen), random_y_pos(gen));
}

void Asteroid::Move(int screen_width, int screen_height, float delta_time_seconds)
{
	SDL_FRect rect = this->GetRect();
	float pos_x = rect.x;
	float pos_y = rect.y;
	float width = rect.w;
	float height = rect.h;

	if (pos_x <= -width)
	{
		pos_x = screen_width;
	}
	else if (pos_x >= screen_width)
	{
		pos_x = -width;
	}
	if (pos_y <= -height)
	{
		pos_y = screen_height;
	}
	else if (pos_y >= screen_height)
	{
		pos_y = -height;
	}

	pos_x += std::sin(Angle * M_PI / 180) * Velocity * delta_time_seconds;
	pos_y -= std::cos(Angle * M_PI / 180) * Velocity * delta_time_seconds;

	this->SetRectPos(pos_x, pos_y);

	return;
}

Asteroid::~Asteroid()
{
	return;
}

void Asteroid::Render()
{
	SDL_RenderCopyExF(destRenderer, Texture, nullptr, &Rect, Angle, nullptr, SDL_FLIP_NONE);
}