#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <string>

typedef unsigned int uint32_t;
typedef uint32_t Uint32;

struct Configuration
{
public:
	//* non-static

	/*------------ Window ------------*/

	int
		START_WINDOW_WIDTH = 1440,
		START_WINDOW_HEIGHT = 810;

	/*------------ Text ------------*/

	std::string FONT_PATH = "/usr/share/fonts/msttcore/comicbd.ttf";
	std::wstring START_TEXT = L"Starten: \"Enter\"\nSchließen: \"Q\"";

	int
		FONT_COLOR_HEX = 0xFFFFFF,
		START_TEXT_SIZE = 100,
		SCORE_TEXT_SIZE = 70;

	/*------------ Player ------------*/

	float
		PLAYER_ACCELEARION = 200,
		PLAYER_MAX_VELOCITY = 600,
		PLAYER_MAX_VELOCITY_BOOST = 10,
		PLAYER_ACCELEARION_BOOST = 10,
		PLAYER_SCALE = 1,
		PLAYER_FRICTION = 200,
		PLAYER_RADIUS = 30;

	int
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

	int
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

	int
		COUNTDOWN_DURATION = 3,
		COUNTDOWN_INTERVAL = 1;

	/*--- Constructor/Destructor ---*/
	Configuration() = default;
	~Configuration() = default;
};

#endif //! GLOBALS_HPP