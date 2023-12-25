#ifndef OBJECT_STORAGE_HPP
#define OBJECT_STORAGE_HPP

#include <string>
#include <vector>
#include <unordered_map>

#include "Sprite/IMGSprite.hpp"
#include "Sprite/Asteroid.hpp"
#include "Sprite/Gem.hpp"
#include "Sprite/Player.hpp"
#include "Sprite/Text.hpp"
#include "Sound/soundChunk.hpp"
#include "Sound/soundMusic.hpp"

struct objectStorage
{
public:
	//* Storages

	std::unordered_map<std::string, IMGSprite> IMGSprites;
	std::unordered_map<std::string, Player> Players;
	std::unordered_map<std::string, Text> Texts;
	std::unordered_map<std::string, soundChunk> Chunks;
	std::unordered_map<std::string, soundMusic> Musics;

	std::vector<Asteroid> Asteroids;
	std::vector<Gem> Gems;

	//* Cleanup

	void ClearAll();

	//* Constructor/Destructor

	objectStorage() = default;
	~objectStorage() = default;
};

#endif //! OBJECT_STORAGE_HPP