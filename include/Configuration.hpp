#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <string>
#include <unordered_map>

#include <SDL2/SDL.h>

typedef unsigned int uint32_t;
typedef uint32_t Uint32;

struct Configuration
{
private:
	//* non-static
	/*------------ Variable Mappings ------------*/

	std::unordered_map<std::string, unsigned int *> UIntMap;
	std::unordered_map<std::string, SDL_Color *> colorMap;
	std::unordered_map<std::string, float *> floatMap;
	std::unordered_map<std::string, std::string *> stringMap;
	std::unordered_map<std::string, std::wstring *> wstringMap;

	/*------------ Settings ------------*/

	std::pair<std::string, std::string> SplitKeyValue(const std::string &string);
	void ApplySetting(const std::string &key, const std::string &value);

public:
	//* non-static

	/*------------ Window ------------*/

	unsigned int
		START_WINDOW_WIDTH = 1440,
		START_WINDOW_HEIGHT = 810;

	/*------------ Text ------------*/

	std::string FONT_PATH = "/usr/share/fonts/msttcore/comicbd.ttf";

	std::wstring START_TEXT = L"Starten: \"Enter\"\nSchließen: \"Q\"";
	std::wstring GAME_OVER_TEXT = L"GAME OVER\nDein Score: {Score} Punkte\nDein Highscore: {Highscore} Punkte";

	SDL_Color START_TEXT_COLOR = {255, 255, 255, 255};
	SDL_Color GAME_OVER_TEXT_COLOR = {255, 0, 0, 255};
	SDL_Color INGAME_TEXT_COLOR = {255, 255, 255, 255};

	unsigned int
		START_TEXT_SIZE = 100,
		GAME_OVER_TEXT_SIZE = 100,
		INGAME_TEXT_SIZE = 70,
		START_SCORE = 0;

	/*------------ Player ------------*/

	float
		PLAYER_ACCELEARION = 200,
		PLAYER_MAX_VELOCITY = 600,
		PLAYER_MAX_VELOCITY_BOOST = 10,
		PLAYER_ACCELEARION_BOOST = 10,
		PLAYER_SCALE = 1,
		PLAYER_FRICTION = 200,
		PLAYER_RADIUS = 30;

	unsigned int
		PLAYER_FRAME_WIDTH = 68,
		PLAYER_FRAME_HEIGHT = 80,
		PLAYER_IMG_FRAMES = 6,
		PLAYER_IMG_TYPES = 4,
		PLAYER_ANIMATIONS_PER_SECOND = 15,
		PLAYER_EFFECT_DURATION_SECONDS = 1,
		PLAYER_ROTATION_SPEED = 144;

	/*------------ Gem ------------*/

	float
		GEM_SCALE = 1,
		GEM_RADIUS = 30;

	unsigned int
		GEM_FRAME_WIDTH = 52,
		GEM_FRAME_HEIGHT = 64,
		GEM_IMG_FRAMES = 16,
		GEM_IMG_TYPES = 8,
		GEM_BLINK_DURATION = 3000,
		GEM_LIFETIME_DURATION = 7000,
		GEM_BLINK_FACTOR = 2,
		GEM_MAXIMUM_BRIGHTNESS = 255,
		GEM_MINIMUM_BRIGHTNESS = 150;

	/*------------ Time ------------*/

	unsigned int
		COUNTDOWN_DURATION_MILLISECONDS = 3000,
		COUNTDOWN_INTERVAL_MILLISECONDS = 1000;

	/*------------ Settings ------------*/

	void ImportSettings();

	/*--- Constructor/Destructor ---*/
	Configuration();
	~Configuration();
};

#endif //! GLOBALS_HPP