#ifndef OBJECT_STORAGE_HPP
#define OBJECT_STORAGE_HPP

#include <string>
#include <vector>
#include <unordered_map>

#include "IMGSprite.hpp"
#include "Asteroid.hpp"
#include "Gem.hpp"
#include "Player.hpp"
#include "Text.hpp"
#include "soundChunk.hpp"
#include "soundMusic.hpp"

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