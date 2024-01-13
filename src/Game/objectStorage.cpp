#include "Game/objectStorage.hpp"

Text &objectStorage::GetText(std::string key)
{
	return TextMap.at(key);
}

void objectStorage::SetText(std::string key, Text &text_sprite)
{
	TextMap.insert({key, text_sprite});

	return;
}

void objectStorage::ClearAll()
{
	TextMap.clear();
	Asteroids.clear();
	Gems.clear();
	Chunks.clear();
	Musics.clear();

	for (auto &pair : gameModes)
	{
		pair.first.clear();
		pair.second.clear();
	}

	return;
}