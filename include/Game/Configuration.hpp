#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <string>
#include <unordered_map>

#include <SDL2/SDL.h>

struct Configuration
{
private:
	std::unordered_map<std::string, unsigned int *> UIntMap;
	std::unordered_map<std::string, float *> floatMap;
	std::unordered_map<std::string, std::string *> stringMap;
	std::unordered_map<std::string, std::wstring *> wstringMap;
	std::unordered_map<std::string, SDL_Color *> colorMap;

	std::pair<std::string, std::string> SplitKeyValue(const std::string &string);
	void ApplySetting(const std::string &key, std::string &value);

public:
	//*  ------------ Window ------------

	unsigned int
		START_WINDOW_WIDTH = 1440,
		START_WINDOW_HEIGHT = 810,

		COUNTDOWN_DURATION_MILLISECONDS = 3000,
		COUNTDOWN_INTERVAL_MILLISECONDS = 1000,

		START_SCORE = 0;

	//*  ------------ Text -------------

	std::string FONT_PATH = "/usr/share/fonts/msttcore/comicbd.ttf";

	std::wstring START_TEXT = L"Starten: \"Enter\"\nSchließen: \"Q\"";
	std::wstring GAME_OVER_TEXT = L"GAME OVER\nDein Score: {S} Punkte\nDein Highscore: {HS} Punkte";

	SDL_Color START_TEXT_COLOR = {255, 255, 255, 255};
	SDL_Color GAME_OVER_TEXT_COLOR = {255, 0, 0, 255};
	SDL_Color INGAME_TEXT_COLOR = {255, 255, 255, 255};

	unsigned int
		START_TEXT_SIZE = 100,
		GAME_OVER_TEXT_SIZE = 100,
		INGAME_TEXT_SIZE = 70;

	//*  ------------ Player ------------

	float
		PLAYER_SCALE = 1;

	unsigned int
		PLAYER_RADIUS = 30,

		PLAYER_FRICTION = 150,
		PLAYER_MAX_VELOCITY = 600,
		PLAYER_MAX_VELOCITY_BOOST = 20,
		PLAYER_ACCELEARION = 200,
		PLAYER_ACCELEARION_BOOST = 20,
		PLAYER_ROTATION_SPEED = 144,

		PLAYER_FRAME_WIDTH = 68,
		PLAYER_FRAME_HEIGHT = 80,
		PLAYER_IMG_FRAMES = 6,
		PLAYER_IMG_TYPES = 4,
		PLAYER_ANIMATIONS_PER_SECOND = 15,
		PLAYER_EFFECT_DURATION_MILLISECONDS = 2000;

	/*------------ Gem ------------*/

	float
		GEM_SCALE = 1;

	unsigned int
		GEM_RADIUS = 30,

		GEM_BLINK_DURATION_MILLISECONDS = 2000,
		GEM_LIFETIME_DURATION_MILLISECONDS = 4500,
		GEM_BLINK_FACTOR = 20,
		GEM_MINIMUM_BRIGHTNESS = 150,
		GEM_MAXIMUM_BRIGHTNESS = 255,

		GEM_FRAME_WIDTH = 52,
		GEM_FRAME_HEIGHT = 64,
		GEM_IMG_FRAMES = 16,
		GEM_IMG_TYPES = 8;

	Configuration();
	~Configuration();

	void ImportSettings();
};

#endif //! GLOBALS_HPP