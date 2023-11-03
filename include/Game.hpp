#ifndef GAME_HPP
#define GAME_HPP

#include "Configuration.hpp"
#include "EventHandler.hpp"
#include "objectStorage.hpp"
#include "Time.hpp"
#include "Sound.hpp"

typedef enum gameState
{
	TITLE_SCREEN,
	GAME_STARTED,
	GAME_PAUSED,
	GAME_OVER
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
		Score = 0,
		Highscore = 0;

	Configuration Config;
	EventHandler gameEvents;

	Clock gameClock;
	SecondTimer startTimer;
	Uint64 currentTicks = 0;
	Uint64 pausedTicks = 0;
	Uint64 totalTimePaused = 0;

	volumeControl volumeController;

	gameState currentGameState;
	gameState lastGameState;

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
	void GameTitleScreen();
	void GameStarted();
	void GamePaused();
	void GameOver();

	//* Game handling
	Uint64 GetCurrentTime();
	void UpdateScore(int amount);
	void HandlePlayers();
	void HandleAsteroids();
	void HandleGems();
	void CheckCollisions();
	void HandleGameOver();

	//* Rendering
	void UpdateRenderScale();
	void RenderObjects(objectStorage *storage);
	void AnimateObjects(objectStorage *storage);

	//* Constructor/Destructor
	Game() = default;
	~Game() = default;
};

#endif //! GAME_HPP