#include "Game/objectStorage.hpp"

void objectStorage::ClearAll()
{
	Texts.clear();
	IMGSprites.clear();
	Asteroids.clear();
	Gems.clear();
	Players.clear();
	Chunks.clear();
	Musics.clear();

	return;
}