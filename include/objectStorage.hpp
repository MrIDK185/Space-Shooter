#ifndef OBJECT_STORAGE_HPP
#define OBJECT_STORAGE_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

class IMGSprite;
class Asteroid;
class Gem;
class Player;
class Text;
class soundChunk;
class soundMusic;

struct objectStorage
{
public:
	//* Storages
	std::unordered_map<std::string, std::shared_ptr<IMGSprite>> IMGSprites;
	std::unordered_map<std::string, std::shared_ptr<Player>> Players;
	std::unordered_map<std::string, std::shared_ptr<Text>> Texts;
	std::unordered_map<std::string, std::shared_ptr<soundChunk>> Chunks;
	std::unordered_map<std::string, std::shared_ptr<soundMusic>> Musics;

	std::vector<std::shared_ptr<Asteroid>> Asteroids;
	std::vector<std::shared_ptr<Gem>> Gems;

	//* Cleanup
	void ClearAll();

	//* Constructor/Destructor
	objectStorage() = default;
	~objectStorage() = default;
};

#endif //! OBJECT_STORAGE_HPP