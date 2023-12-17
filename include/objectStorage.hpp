#ifndef OBJECT_STORAGE_HPP
#define OBJECT_STORAGE_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

#include "IMGSprite.hpp"
#include "Asteroid.hpp"
#include "Gem.hpp"
#include "Player.hpp"
#include "Text.hpp"
#include "Sound.hpp"

/*class IMGSprite;
class Asteroid;
class Gem;
class Player;
class Text;
class soundChunk;
class soundMusic;*/

struct objectStorage
{
public:
	//* Storages
	std::unordered_map<std::string, std::unique_ptr<IMGSprite>> IMGSprites;
	std::unordered_map<std::string, std::unique_ptr<Player>> Players;
	std::unordered_map<std::string, std::unique_ptr<Text>> Texts;
	std::unordered_map<std::string, std::unique_ptr<soundChunk>> Chunks;
	std::unordered_map<std::string, std::unique_ptr<soundMusic>> Musics;

	std::vector<std::unique_ptr<Asteroid>> Asteroids;
	std::vector<std::unique_ptr<Gem>> Gems;

	//* Cleanup
	void ClearAll();

	//* Constructor/Destructor
	objectStorage() = default;
	~objectStorage() = default;
};

#endif //! OBJECT_STORAGE_HPP