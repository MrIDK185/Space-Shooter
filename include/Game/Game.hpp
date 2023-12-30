#ifndef GAME_HPP
#define GAME_HPP

#include "Game/Configuration.hpp"
#include "Game/EventHandler.hpp"
#include "Game/objectStorage.hpp"
#include "Game/gameState.hpp"
#include "Time/Clock.hpp"
#include "Time/Timer.hpp"
#include "Sound/volumeControl.hpp"
#include "Sound/channelControl.hpp"

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
	Timer startTimer;
	Uint64 currentTicks = 0;
	Uint64 pausedTicks = 0;
	Uint64 totalTimePaused = 0;

	volumeControl volumeController;
	channelControl channelController;

	gameState currentGameState;
	gameState lastGameState;

	bool Running = true;

	objectStorage
		objectsTitleScreen,
		objectsGameRunning,
		objectsGamePaused,
		objectsGameOver;

	std::pair<SpriteData, AsteroidData> asteroidDataDefault;
	std::pair<AnimationData, GemData> gemDataDefault;
	std::pair<AnimationData, PlayerData> playerDataDefault;

	//* Main loop

	void Run();

	//* Cleanup

	void Cleanup();

	//* Initialization

	void SetupGame();
	void InitializeDefaultObjectData();
	void CreateObjectsTitleScreen();
	void CreateObjectsGameRunning();
	void CreateObjectsGamePaused();
	void CreateObjectsGameOver();
	void CreateObjects();
	void Reset();

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