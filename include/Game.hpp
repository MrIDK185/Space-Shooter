#ifndef GAME_HPP
#define GAME_HPP

#include "Configuration.hpp"
#include "EventHandler.hpp"
#include "objectStorage.hpp"
#include "Time.hpp"
#include "Sound.hpp"

typedef enum
{
	GAME_STARTED,
	GAME_PAUSED,
	TITLE_SCREEN
} gameState;

struct Game
{
public:
	//* Variables
	SDL_Window *Window = nullptr;
	SDL_Renderer *Renderer = nullptr;
	const Uint8 *Keyboard = nullptr;

	unsigned int
		screenWidth = 1920,
		screenHeight = 1080,
		Score = 0;

	Configuration Config;
	EventHandler gameEvents;

	Clock gameClock;
	SecondTimer startTimer;
	Uint64 currentTicks = 0;

	volumeControl volumeController;

	gameState currentGameState = TITLE_SCREEN;

	bool Running = true;

	objectStorage
		objectsTitleScreen,
		objectsGameRunning,
		objectsGamePaused,
		objectsGameOver;

	//* Main loop
	int Run();

	//* Cleanup
	void Cleanup();

	//* Initialization
	void SetupGame();
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