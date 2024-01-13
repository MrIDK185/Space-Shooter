#ifndef OBJECT_STORAGE_HPP
#define OBJECT_STORAGE_HPP

#include <string>
#include <vector>
#include <array>
#include <unordered_map>

#include "Game/gameState.hpp"
#include "Sprite/Text.hpp"
#include "Sprite/IMGSprite.hpp"
#include "Sprite/Asteroid.hpp"
#include "Sprite/Gem.hpp"
#include "Sprite/Player.hpp"
#include "Sound/soundChunk.hpp"
#include "Sound/soundMusic.hpp"

struct objectStorage
{
private:
	std::unordered_map<std::string, Text &> TextMap;

public:
	std::vector<Asteroid> Asteroids;
	std::vector<Gem> Gems;
	Player Player;

	std::unordered_map<std::string, soundChunk> Chunks;
	std::unordered_map<std::string, soundMusic> Musics;

	std::array<std::pair<std::vector<Text>,
						 std::vector<IMGSprite>>,
			   NUM_GAME_STATES>
		gameModes;

	objectStorage() = default;
	~objectStorage() = default;

	Text &GetText(std::string key);

	void SetText(std::string key, Text &text_sprite);

	void ClearAll();
};

#endif //! OBJECT_STORAGE_HPP