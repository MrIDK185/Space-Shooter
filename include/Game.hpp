#ifndef GAME_HPP
#define GAME_HPP

#include "Configuration.hpp"
#include "EventHandler.hpp"
#include "Time.hpp"

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

typedef unsigned char uint8_t;
typedef uint8_t Uint8;

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

	int
		screenWidth = 1920,
		screenHeight = 1080;

	Configuration Config;
	EventHandler gameEvents;

	Clock gameClock;
	SecondTimer startTimer;
	Uint64 currentTicks = 0;

	typedef enum
	{
		GAME_STARTED,
		GAME_PAUSED,
		TITLE_SCREEN
	} gameState;

	typedef enum
	{
		UNMUTED,
		MUTED
	} soundState;

	gameState currentGameState = TITLE_SCREEN;
	soundState currentSoundState = UNMUTED;

	bool
		Running = true,
		startCountdown = false,	   // Probably going to be removed
		startSoundPlaying = false; // Probably going to be removed

	int
		Score = 0,
		Countdown = 3,
		currentChunkVolume = 0,
		lastChunkVolume = 0;

	std::unordered_map<std::string, std::shared_ptr<IMGSprite>> IMGSpriteMap;
	std::unordered_map<std::string, std::shared_ptr<Player>> playerMap;
	std::unordered_map<std::string, std::shared_ptr<Text>> textMap;
	std::unordered_map<std::string, std::shared_ptr<soundChunk>> chunkMap;
	std::unordered_map<std::string, std::shared_ptr<soundMusic>> musicMap;

	std::vector<std::shared_ptr<Gem>> gemGroup;
	std::vector<std::shared_ptr<Asteroid>> asteroidGroup;

	//* Main loop
	int Run();

	//* Error-handling
	void ShowErrorMessage();

	//* Cleanup
	void Clean();
	void Quit();

	//* Initialization
	int InitializeSystems();
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
	template <typename valueType>
	void RenderMap(std::unordered_map<std::string, std::shared_ptr<valueType>> *map);
	template <typename elementType>
	void RenderVector(std::vector<std::shared_ptr<elementType>> *vector);
	template <typename valueType>
	void AnimateMap(std::unordered_map<std::string, std::shared_ptr<valueType>> *map);

	//* Constructor/Destructor
	Game() = default;
	~Game() = default;
};

#endif //! GAME_HPP