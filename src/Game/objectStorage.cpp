#include "objectStorage.hpp"

//* public

void objectStorage::ClearAll()
{
	IMGSprites.clear();
	Players.clear();
	Texts.clear();
	Chunks.clear();
	Musics.clear();
	Gems.clear();
	Asteroids.clear();

	return;
}