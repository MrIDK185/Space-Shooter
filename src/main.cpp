#define SDL_MAIN_HANDLED

#include "Globals.hpp"
#include "EventHandler.hpp"
#include <Asteroid.hpp>
#include "Player.hpp"
#include "Gem.hpp"
#include "Text.hpp"
#include "Sound.hpp"
#include "Time.hpp"

#include <iostream>
#include <fstream>
#include <codecvt>
#include <boost/algorithm/string/trim.hpp>

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

//* Error-handling
void ShowErrorMessage();

//* Cleanup
void Clean(Configuration &config);
void Quit(Configuration &config);

//* Initialization
int InitializeSystems(Configuration &config);
void SetupGame(Configuration &config);
void ImportSettings(const std::string &filepath, Configuration &config);
void CreateObjects(Configuration &config);

//* Game modes
void GamePaused(Configuration &config);
void GameStarted(Configuration &config);
void GameTitleScreen(Configuration &config);

//* Game handling
void HandlePlayers(Configuration &config);
void HandleGems(Configuration &config);
void HandleAsteroids(Configuration &config);
void CheckCollisions(Configuration &config);

//* Rendering
template <typename valueType>
void RenderMap(std::map<std::string, std::shared_ptr<valueType>> *map);
template <typename elementType>
void RenderVector(std::vector<std::shared_ptr<elementType>> *vector);
template <typename valueType>
void AnimateMap(std::map<std::string, std::shared_ptr<valueType>> *map, Uint64 current_ticks);

int main()
{
	Configuration config;

	if (InitializeSystems(config) < 0)
	{
		return 1;
	}

	SetupGame(config);

	std::cout << "------- Startup successful --------\n\n";
	auto gameEvents = EventHandler(config);

	while (config.Running)
	{
		gameEvents.HandleEvents();

		config.gameClock.Tick();
		config.currentTicks = SDL_GetTicks64();
		config.Keyboard = SDL_GetKeyboardState(nullptr);

		config.currentChunkVolume = Mix_MasterVolume(-1);

		SDL_GetWindowSizeInPixels(config.Window, &config.WINDOW_WIDTH, &config.WINDOW_HEIGHT);
		config.WINDOW_WIDTH_SCALE = static_cast<float>(config.WINDOW_WIDTH) / static_cast<float>(config.SCREEN_RESOLUTION_WIDTH);
		config.WINDOW_HEIGHT_SCALE = static_cast<float>(config.WINDOW_HEIGHT) / static_cast<float>(config.SCREEN_RESOLUTION_HEIGHT);

		SDL_RenderSetScale(config.Renderer, config.WINDOW_WIDTH_SCALE, config.WINDOW_HEIGHT_SCALE);

		if (config.gamePaused)
		{
			GamePaused(config);
		}
		else if (config.gameStarted)
		{
			GameStarted(config);
		}
		else
		{
			GameTitleScreen(config);
		}
	}

	Quit(config);
	return 0;
}

//* Error-handling
void ShowErrorMessage()
{
	const char *error_msg = SDL_GetError();
	if (int displayed = SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Initialization error", error_msg, nullptr); displayed < 0)
	{
		std::cout << "Initialization error: " << error_msg << "\n";
		std::cout << "MessageBox could not be displayed: " << SDL_GetError() << "\n";
	}
	error_msg = nullptr;

	return;
}

//* Cleanup
void Clean(Configuration &config)
{

	std::cout << "Cleaning Player default values...\n";
	Player::CleanDefaultValues();
	std::cout << "Cleaning Gem default values...\n\n";
	Gem::CleanDefaultValues();

	std::cout << "Clearing musicMap...\n";
	config.musicMap.clear();
	std::cout << "Clearing chunkMap...\n";
	config.chunkMap.clear();
	std::cout << "Clearing textMap...\n";
	config.textMap.clear();
	std::cout << "Clearing playerMap...\n";
	config.playerMap.clear();
	std::cout << "Clearing gemGroup...\n";
	config.gemGroup.clear();
	std::cout << "Clearing asteroidGroup...\n";
	config.asteroidGroup.clear();
	std::cout << "Clearing IMGSpriteMap...\n\n";
	config.IMGSpriteMap.clear();

	std::cout << "Destroying renderer...\n";
	SDL_DestroyRenderer(config.Renderer);
	config.Renderer = nullptr;
	std::cout << "Destroying main window...\n";
	SDL_DestroyWindow(config.Window);
	config.Window = nullptr;

	std::cout << "Destroying keyboard...\n\n";
	config.Keyboard = nullptr;

	return;
}

void Quit(Configuration &config)
{
	std::cout << "\n------------- Cleanup -------------\n\n";
	Clean(config);

	std::cout << "Closing audio device...\n";
	Mix_CloseAudio();
	std::cout << "Quitting Mixer...\n";
	Mix_Quit();
	std::cout << "Quitting TTF...\n";
	TTF_Quit();
	std::cout << "Quitting IMG...\n";
	IMG_Quit();
	std::cout << "Quitting SDL...\n\n";
	SDL_Quit();
	std::cout << "-------- Cleanup successful --------\n";

	return;
}

//* Initialization
int InitializeSystems(Configuration &config)
{
	std::cout << "------------- Startup -------------\n\n";

	std::cout << "Initializing SDL...\n";
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		ShowErrorMessage();
		Quit(config);
		return -1;
	}
	std::cout << "Initializing IMG...\n";
	if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) != (IMG_INIT_JPG | IMG_INIT_PNG))
	{
		ShowErrorMessage();
		Quit(config);
		return -1;
	}
	std::cout << "Initializing TTF...\n";
	if (TTF_Init() < 0)
	{
		ShowErrorMessage();
		Quit(config);
		return -1;
	}
	std::cout << "Initializing Mixer...\n";
	if (Mix_Init(MIX_INIT_FLAC | MIX_INIT_MP3) != (MIX_INIT_FLAC | MIX_INIT_MP3))
	{
		ShowErrorMessage();
		Quit(config);
		return -1;
	}
	std::cout << "Opening audio device...\n\n";
	if (Mix_OpenAudio(48000, AUDIO_F32SYS, 2, 4096) < 0)
	{
		ShowErrorMessage();
		Quit(config);
		return -1;
	}

	return 0;
}

void SetupGame(Configuration &config)
{
	std::cout << "Importing settings...\n";
	ImportSettings("config/settings.txt", config);

	SDL_GetWindowSizeInPixels(config.Window, &config.WINDOW_WIDTH, &config.WINDOW_HEIGHT);
	SDL_DisplayMode display_mode;
	SDL_GetCurrentDisplayMode(0, &display_mode);
	config.SCREEN_RESOLUTION_WIDTH = display_mode.w;
	config.SCREEN_RESOLUTION_HEIGHT = display_mode.h;
	config.windowFlags = SDL_GetWindowFlags(config.Window);

	config.currentChunkVolume = Mix_MasterVolume(-1);
	config.lastChunkVolume = config.currentChunkVolume;

	config.startTimer.Duration = config.Countdown;
	config.startTimer.Interval = config.countdownInterval;

	std::cout << "Creating main window...\n";
	config.Window = SDL_CreateWindow("Space Shooter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, config.WINDOW_WIDTH, config.WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
	std::cout << "Creating main renderer...\n\n";
	config.Renderer = SDL_CreateRenderer(config.Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	std::cout << "Creating objects...\n";
	CreateObjects(config);

	return;
}

std::pair<std::string, std::string> SplitKeyValue(std::string_view string)
{
	std::string key;
	std::string value;
	size_t index = string.find("=");

	key = string.substr(0, index);
	boost::algorithm::trim(key);

	value = string.substr(index + 1, string.length() - (index + 1));
	boost::algorithm::trim(value);
	if (size_t idx = value.find("\\n"); idx != std::string::npos)
	{
		value.replace(idx, 2, "\n");
	}

	return std::pair<std::string, std::string>(key, value);
}

void ImportSettings(const std::string &filepath, Configuration &config)
{
	std::map<std::string, int> intMap = {
		{"Window Width", config.WINDOW_WIDTH},
		{"Window Height", config.WINDOW_HEIGHT},

		{"Countdown", config.Countdown},
		{"Countdown Interval", config.countdownInterval},

		{"Start Text Size", config.startTextSize},
		{"Score Text Size", config.scoreTextSize},

		{"Player Frame Width", config.playerFrameWidth},
		{"Player Frame Height", config.playerFrameHeight},
		{"Player IMG Frames", config.playerIMGFrames},
		{"Player IMG Types", config.playerIMGTypes},
		{"Player Animations Per Second", config.playerAnimationsPerSecond},
		{"Player Effect Duration Seconds", config.playerEffectDurationSeconds},
		{"Player Rotation Speed", config.playerRotationSpeed},
		{"Player Max Velocity Boost", config.playerMaxVelocityBoost},
		{"Player Acceleration Boost", config.playerAccelerationBoost},

		{"Gem Frame Width", config.gemFrameWidth},
		{"Gem Frame Height", config.gemFrameHeight},
		{"Gem IMG Frames", config.gemIMGFrames},
		{"Gem IMG Types", config.gemIMGTypes},
		{"Gem Blink Duration", config.gemBlinkDuration},
		{"Gem Lifetime Duration", config.gemLifetimeDuration},
		{"Gem Maximum Brightness", config.gemMaximumBrightness},
		{"Gem Minimum Brightness", config.gemMinimumBrightness},
	};

	std::map<std::string, float> floatMap = {
		{"Player Acceleration", config.playerAcceleration},
		{"Player Max Velocity", config.playerMaxVelocity},
		{"Player Friction", config.playerFriction},
		{"Player Scale", config.playerScale},
		{"Player Radius", config.playerRadius},

		{"Gem Scale", config.gemScale},
		{"Gem Radius", config.gemRadius},
		{"Gem Blink Factor", config.gemBlinkFactor},
	};

	std::map<std::string, std::string> stringMap = {
		{"Font Path", config.fontPath},
	};

	std::map<std::string, std::wstring> wstringMap = {
		{"Start Text", config.startText},
	};

	std::cout << "Opening settings file...\n";
	std::ifstream file(filepath, std::ios_base::in);
	std::string line;
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

	if (!file)
	{
		std::cout << "Closing settings file...\n\n";
		file.close();

		intMap.clear();
		floatMap.clear();
		stringMap.clear();
		wstringMap.clear();

		return;
	}

	while (std::getline(file, line))
	{
		if (line.empty())
		{
			continue;
		}

		const auto [key, value] = SplitKeyValue(line);

		try
		{
			if (key == "Font Color")
			{
				std::cout << "Initializing " << key << "...\n";
				int start_pos = value[0] == '#' ? 1 : 0;
				config.fontColorHex = std::stoi(value.substr(start_pos), nullptr, 16);
				continue;
			}

			if (auto itInt = intMap.find(key); itInt != intMap.end())
			{
				std::cout << "Initializing " << key << " ... \n";
				itInt->second = std::stoi(value);
				continue;
			}

			if (auto itFloat = floatMap.find(key); itFloat != floatMap.end())
			{
				std::cout << "Initializing " << key << " ... \n";
				itFloat->second = std::stof(value);
				continue;
			}

			if (auto itString = stringMap.find(key); itString != stringMap.end())
			{
				std::cout << "Initializing " << key << "...\n";
				itString->second = value;
				continue;
			}

			auto itWString = wstringMap.find(key);
			if (itWString != wstringMap.end())
			{
				std::cout << "Initializing " << key << "...\n";
				itWString->second = converter.from_bytes(value);
				continue;
			}
		}

		catch (const std::invalid_argument &)
		{
			continue;
		}
		catch (const std::out_of_range &)
		{
			continue;
		}
	}

	std::cout << "Closing settings file...\n\n";
	file.close();

	intMap.clear();
	floatMap.clear();
	stringMap.clear();
	wstringMap.clear();

	return;
}

void CreateObjects(Configuration &config)
{
	config.IMGSpriteMap["startBackground"] = std::make_shared<IMGSprite>(config.Renderer, "assets/images/background_blurred.png", 1, 0);

	Uint8 r;
	Uint8 g;
	Uint8 b;
	r = ((config.fontColorHex >> 16) & 0xFF);
	g = ((config.fontColorHex >> 8) & 0xFF);
	b = (config.fontColorHex & 0xFF);
	SDL_Color font_color = {r, g, b, 255};

	config.textMap["startText"] = std::make_shared<Text>(config.Renderer, config.startText.c_str(), config.fontPath.c_str(), config.startTextSize, font_color);
	SDL_FRect text_rect = config.textMap.at("startText")->GetRect();
	config.textMap.at("startText")->SetRectPos(static_cast<float>(config.SCREEN_RESOLUTION_WIDTH / 2) - text_rect.w / 2, static_cast<float>(config.SCREEN_RESOLUTION_HEIGHT / 2) - text_rect.h / 2);

	config.IMGSpriteMap["gameBackground"] = std::make_shared<IMGSprite>(config.Renderer, "assets/images/background.png", 1, 0);

	config.asteroidGroup.push_back(std::make_shared<Asteroid>(config.Renderer, "assets/images/asteroid1.png", 1, 60, 200, config));

	std::cout << "Initializing Gem default values...\n";
	Gem::InitDefaultValues(config.Renderer, "assets/images/gems.png", config.gemScale, config.gemRadius, config.gemFrameWidth, config.gemFrameHeight, config.gemIMGFrames, config.gemIMGTypes, 0, config.gemBlinkDuration, config.gemLifetimeDuration);
	config.gemGroup.push_back(Gem::NewGem(config));

	std::cout << "Initializing Player default values...\n\n";
	Player::InitDefaultValues(config.Renderer, config.playerScale, config.playerRadius, config.playerFrameWidth, config.playerFrameHeight, config.playerIMGFrames, config.playerIMGTypes, config.playerAnimationsPerSecond, config.playerAcceleration, config.playerMaxVelocity, config.playerFriction, config.playerEffectDurationSeconds, config.playerRotationSpeed);
	config.playerMap["Player1"] = std::make_shared<Player>("assets/images/player.png", config);
	SDL_Rect player_rect = config.playerMap.at("Player1")->GetIMGPartRect();
	config.playerMap.at("Player1")->SetRectPos(static_cast<float>(config.SCREEN_RESOLUTION_WIDTH / 2) - static_cast<float>(player_rect.w / 2), static_cast<float>(config.SCREEN_RESOLUTION_HEIGHT / 2) - static_cast<float>(player_rect.h / 2));

	const wchar_t score_text[] = {L"0"};
	config.textMap["scoreText"] = std::make_shared<Text>(config.Renderer, score_text, config.fontPath.c_str(), config.scoreTextSize, font_color);
	config.textMap.at("scoreText")->SetRectPos(static_cast<float>(config.SCREEN_RESOLUTION_WIDTH) - config.textMap.at("scoreText")->GetRect().w, 0);

	config.chunkMap["startSound"] = std::make_shared<soundChunk>("assets/sounds/game_start.mp3");

	config.chunkMap["gemCollected"] = std::make_shared<soundChunk>("assets/sounds/gem_collected.mp3");

	config.chunkMap["gemMissed"] = std::make_shared<soundChunk>("assets/sounds/gem_missed.wav");

	config.musicMap["menuMusic"] = std::make_shared<soundMusic>("assets/sounds/menu_music.mp3");
	config.musicMap.at("menuMusic")->PlayMusic();

	config.musicMap["backgroundMusic"] = std::make_shared<soundMusic>("assets/sounds/background_music.mp3");

	Mix_VolumeMusic(Mix_MasterVolume(-1) / 5);

	return;
}

//* Game modes
void GamePaused(Configuration &config)
{
	SDL_RenderPresent(config.Renderer);

	return;
}

void GameStarted(Configuration &config)
{
	if (config.startSoundPlaying && !Mix_Playing(config.chunkMap.at("startSound")->GetChannel()))
	{
		config.musicMap.at("backgroundMusic")->PlayMusic();
		config.startSoundPlaying = false;
	}

	HandlePlayers(config);
	HandleGems(config);
	HandleAsteroids(config);
	CheckCollisions(config);
	AnimateMap<Player>(&config.playerMap, config.currentTicks);

	SDL_RenderClear(config.Renderer);

	RenderMap<IMGSprite>(&config.IMGSpriteMap);
	RenderVector<Gem>(&config.gemGroup);
	RenderMap<Player>(&config.playerMap);
	RenderVector<Asteroid>(&config.asteroidGroup);
	RenderMap<Text>(&config.textMap);

	SDL_RenderPresent(config.Renderer);

	return;
}

void GameTitleScreen(Configuration &config)
{
	SDL_RenderClear(config.Renderer);

	config.IMGSpriteMap.at("startBackground")->Render();
	config.textMap.at("startText")->Render();

	SDL_RenderPresent(config.Renderer);

	if (!config.startCountdown)
	{
		return;
	}

	if (config.startTimer.Check(config.currentTicks))
	{
		config.Countdown -= config.startTimer.Interval;

		std::wstring countdown_str = std::to_wstring(config.Countdown);
		config.textMap.at("startText")->SetMessage(countdown_str.c_str());

		config.textMap.at("scoreText")->SetRectPos(static_cast<float>(config.SCREEN_RESOLUTION_WIDTH) - config.textMap.at("scoreText")->GetRect().w, 0);
	}
	if (config.Countdown <= 0)
	{
		config.startTimer.Stop();
		config.startCountdown = false;
		config.gameStarted = true;

		config.IMGSpriteMap.erase("startBackground");
		config.textMap.erase("startText");

		for (std::shared_ptr<Gem> gem : config.gemGroup)
		{
			gem->SetTicks(config.currentTicks);
		}

		Mix_HaltMusic();
		config.chunkMap.at("startSound")->PlayChunk();
		config.startSoundPlaying = true;

		return;
	}

	return;
}

//* Game handling
void HandlePlayers(Configuration &config)
{
	for (const auto &[name, player] : config.playerMap)
	{
		player->CheckType(config.currentTicks);
		player->HandleKeys();
	}

	return;
}

void HandleGems(Configuration &config)
{
	for (const std::shared_ptr<Gem> &gem : config.gemGroup)
	{
		if (config.currentTicks >= gem->GetBlinkTicks())
		{
			gem->Blink();
		}
		if (config.currentTicks >= gem->GetLifetimeTicks())
		{
			config.Score -= 1;
			config.chunkMap.at("gemMissed")->PlayChunk();

			std::wstring current_score = std::to_wstring(config.Score);
			config.textMap.at("scoreText")->SetMessage(current_score.c_str());

			config.textMap.at("scoreText")->SetRectPos(static_cast<float>(config.SCREEN_RESOLUTION_WIDTH) - config.textMap.at("scoreText")->GetRect().w, 0);

			gem->Randomize();
		}
	}

	return;
}

void HandleAsteroids(Configuration &config)
{
	for (const std::shared_ptr<Asteroid> &asteroid : config.asteroidGroup)
	{
		asteroid->Move();
	}

	return;
}

void CheckCollisions(Configuration &config)
{
	for (const auto &[name, player] : config.playerMap)
	{
		for (const std::shared_ptr<Gem> &gem : config.gemGroup)
		{
			if (!gem->Collideswith(player))
			{
				continue;
			}

			config.Score += 1;
			config.chunkMap.at("gemCollected")->PlayChunk();

			std::wstring current_score = std::to_wstring(config.Score);
			config.textMap.at("scoreText")->SetMessage(current_score.c_str());

			config.textMap.at("scoreText")->SetRectPos(static_cast<float>(config.SCREEN_RESOLUTION_WIDTH) - config.textMap.at("scoreText")->GetRect().w, 0);

			gem->Randomize();

			if (!player->GetGemCollected())
			{
				player->SetMaxVelocity(player->GetMaxVelocity() + config.playerMaxVelocityBoost);
				player->SetAcceleration(player->GetAcceleration() + config.playerAccelerationBoost);
				player->SetGemCollected(true);
			}

			player->SetCollectionTicks(config.currentTicks);
		}

		for (const std::shared_ptr<Asteroid> &asteroid : config.asteroidGroup)
		{
			if (!asteroid->Collideswith(player))
			{
				continue;
			}

			config.Score -= 1;

			std::wstring current_score = std::to_wstring(config.Score);
			config.textMap.at("scoreText")->SetMessage(current_score.c_str());

			config.textMap.at("scoreText")->SetRectPos(static_cast<float>(config.SCREEN_RESOLUTION_WIDTH) - config.textMap.at("scoreText")->GetRect().w, 0);
		}
	}

	return;
}

//* Rendering
template <typename valueType>
void RenderMap(std::map<std::string, std::shared_ptr<valueType>> *map)
{
	for (const auto &[name, element] : *map)
	{
		element->Render();
	}
}

template <typename elementType>
void RenderVector(std::vector<std::shared_ptr<elementType>> *vector)
{
	for (const auto &element : *vector)
	{
		element->Render();
	}
}

template <typename valueType>
void AnimateMap(std::map<std::string, std::shared_ptr<valueType>> *map, Uint64 current_ticks)
{
	for (const auto &[name, element] : *map)
	{
		element->Animate(current_ticks);
	}
}
