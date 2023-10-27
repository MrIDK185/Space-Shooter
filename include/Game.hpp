#ifndef GAME_HPP
#define GAME_HPP

#include "Configuration.hpp"
#include "EventHandler.hpp"
#include "Time.hpp"
#include "Sound.hpp"

#include <vector>
#include <unordered_map>
#include <memory>

typedef enum
{
	GAME_STARTED,
	GAME_PAUSED,
	TITLE_SCREEN
} gameState;

class IMGSprite;
class Asteroid;
class Gem;
class Player;
class Text;
class soundChunk;
class soundMusic;

struct Game
{
	//* Variables
	SDL_Window *Window = nullptr;
	SDL_Renderer *Renderer = nullptr;
	const Uint8 *Keyboard = nullptr;

	unsigned int
		screenWidth = 1920,
		screenHeight = 1080;

	Configuration Config;
	EventHandler gameEvents;

	Clock gameClock;
	SecondTimer startTimer;
	Uint64 currentTicks = 0;

	volumeControl volumeController;

	gameState currentGameState = TITLE_SCREEN;

	bool
		Running = true,
		startSoundPlaying = false; // Probably going to be removed

	unsigned int Score = 0;

	std::unordered_map<std::string, std::shared_ptr<IMGSprite>> IMGSpriteMap;
	std::unordered_map<std::string, std::shared_ptr<Player>> playerMap;
	std::unordered_map<std::string, std::shared_ptr<Text>> textMap;
	std::unordered_map<std::string, std::shared_ptr<soundChunk>> chunkMap;
	std::unordered_map<std::string, std::shared_ptr<soundMusic>> musicMap;

	std::vector<std::shared_ptr<Gem>> gemGroup;
	std::vector<std::shared_ptr<Asteroid>> asteroidGroup;

	//* Main loop
	int Run();

	//* Cleanup
	void Cleanup();

	//* Initialization
	void SetupGame();
	void ImportSettings();
	void CreateObjects();

	//* Game modes
	void GamePaused();
	void GameStarted();
	void GameTitleScreen();

	//* Game handling
	void HandlePlayers();
	void HandleGems();
	void HandleAsteroids();
	void CheckCollisions();

	//* Rendering
	void UpdateRenderScale();
	template <typename keyType, typename valueType>
	void RenderUnorderedMap(std::unordered_map<keyType, valueType> *map);
	template <typename elementType>
	void RenderVector(std::vector<elementType> *vector);
	template <typename keyType, typename valueType>
	void AnimateUnorderedMap(std::unordered_map<keyType, valueType> *map);

	//* Constructor/Destructor
	Game() = default;
	~Game() = default;
};

#endif //! GAME_HPP