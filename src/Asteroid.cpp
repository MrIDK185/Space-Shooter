#include <Asteroid.hpp>

#include <random>

Asteroid::Asteroid(SDL_Renderer *renderer, const char *path, float scale, float radius) : IMGSprite::IMGSprite(renderer, path, scale, radius)
{
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution<> random_angle(0, 359);

	Angle = static_cast<unsigned int>(random_angle(gen));
}

Asteroid::~Asteroid()
{
	return;
}

void Asteroid::Render()
{
	SDL_RenderCopyExF(destRenderer, Texture, nullptr, &Rect, Angle, nullptr, SDL_FLIP_NONE);
}