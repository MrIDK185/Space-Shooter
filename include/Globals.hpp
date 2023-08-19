#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include "Time.hpp"

#include <string>
#include <memory>
#include <vector>
#include <map>

typedef struct SDL_Window SDL_Window;
typedef struct SDL_Renderer SDL_Renderer;
typedef uint8_t Uint8;
typedef uint32_t Uint32;
typedef uint64_t Uint64;

class IMGSprite;
class Asteroid;
class Player;
class Gem;
class Text;
class soundChunk;
class soundMusic;

struct Configuration
{
public:
	//* non-static

	/*------------ Window ------------*/

	int
		WINDOW_WIDTH = 1440,
		WINDOW_HEIGHT = 810,
		SCREEN_RESOLUTION_WIDTH = 1920,
		SCREEN_RESOLUTION_HEIGHT = 1080;

	float
		WINDOW_WIDTH_SCALE = static_cast<float>(WINDOW_WIDTH) / static_cast<float>(SCREEN_RESOLUTION_WIDTH),
		WINDOW_HEIGHT_SCALE = static_cast<float>(WINDOW_HEIGHT) / static_cast<float>(SCREEN_RESOLUTION_HEIGHT);

	SDL_Window *Window = nullptr;
	SDL_Renderer *Renderer = nullptr;

	Uint32 windowFlags = 0;

	/*------------ Text ------------*/

	std::string fontPath = "/usr/share/fonts/msttcore/comicbd.ttf";
	std::wstring startText = L"Starten: \"Enter\"\nSchließen: \"Q\"";

	int
		fontColorHex = 0xFFFFFF,
		startTextSize = 100,
		scoreTextSize = 70;

	/*------------ Player ------------*/

	float
		playerAcceleration = 200,
		playerMaxVelocity = 600,
		playerMaxVelocityBoost = 10,
		playerAccelerationBoost = 10,
		playerScale = 1,
		playerFriction = 200,
		playerRadius = 30;

	int
		playerFrameWidth = 68,
		playerFrameHeight = 80,
		playerIMGFrames = 6,
		playerIMGTypes = 4,
		playerAnimationsPerSecond = 15,
		playerEffectDurationSeconds = 1,
		playerRotationSpeed = 144;

	/*------------ Gem ------------*/

	float
		gemScale = 1,
		gemRadius = 30;

	int
		gemFrameWidth = 52,
		gemFrameHeight = 64,
		gemIMGFrames = 16,
		gemIMGTypes = 8,
		gemBlinkDuration = 3000,
		gemLifetimeDuration = 7000,
		gemBlinkFactor = 2,
		gemMaximumBrightness = 255,
		gemMinimumBrightness = 150;

	/*------------ Time ------------*/

	int
		Countdown = 3,
		countdownInterval = 1;

	Clock gameClock;
	SecondTimer startTimer;
	Uint64 currentTicks = 0;

	/*------------ Game Objects ------------*/

	std::map<std::string, std::shared_ptr<IMGSprite>> IMGSpriteMap;
	std::map<std::string, std::shared_ptr<Player>> playerMap;
	std::map<std::string, std::shared_ptr<Text>> textMap;
	std::map<std::string, std::shared_ptr<soundChunk>> chunkMap;
	std::map<std::string, std::shared_ptr<soundMusic>> musicMap;

	std::vector<std::shared_ptr<Gem>> gemGroup;
	std::vector<std::shared_ptr<Asteroid>> asteroidGroup;

	/*------------ Game(General) ------------*/

	bool
		Running = true,
		gameStarted = false,
		gamePaused = false,
		startCountdown = false,
		Muted = false,
		startSoundPlaying = false;

	int
		Score = 0,
		currentChunkVolume = 0,
		lastChunkVolume = 0;

	const Uint8 *Keyboard = nullptr;

	Configuration() = default;
	~Configuration() = default;
};

#endif //! GLOBALS_HPP