#include "Game.hpp"
#include "Asteroid.hpp"
#include "Player.hpp"
#include "Gem.hpp"
#include "Text.hpp"
#include "Sound.hpp"

#include <iostream>
#include <fstream>
#include <codecvt>
#include <boost/algorithm/string.hpp>

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

int Game::runMainLoop()
{
	if (InitializeSystems() < 0)
	{
		return 1;
	}

	SetupGame();

	std::cout << "------- Startup successful --------\n\n";

	while (Running)
	{
		gameEvents.HandleEvents();

		gameClock.Tick();
		currentTicks = SDL_GetTicks64();
		Keyboard = SDL_GetKeyboardState(nullptr);

		currentChunkVolume = Mix_MasterVolume(-1);

		int current_window_width, current_window_height;
		SDL_GetWindowSizeInPixels(Window, &current_window_width, &current_window_height);
		float window_width_scale = static_cast<float>(current_window_width) / static_cast<float>(screenWidth);
		float window_height_scale = static_cast<float>(current_window_height) / static_cast<float>(screenHeight);

		SDL_RenderSetScale(Renderer, window_width_scale, window_height_scale);

		switch (currentGameState)
		{
		case GAME_PAUSED:
			GamePaused();
			break;

		case GAME_STARTED:
			GameStarted();
			break;

		case TITLE_SCREEN:
			GameTitleScreen();
			break;

		default:
			break;
		}
	}

	Quit();
	return 0;
}

//* Error-handling
void Game::ShowErrorMessage()
{
	const char *error_msg = SDL_GetError();
	int displayed = SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Initialization error", error_msg, nullptr);
	if (displayed < 0)
	{
		std::cout << "Initialization error: " << error_msg << "\n";
		std::cout << "MessageBox could not be displayed: " << SDL_GetError() << "\n";
	}
	error_msg = nullptr;

	return;
}

//* Cleanup
void Game::Clean()
{

	std::cout << "Cleaning Player default values...\n";
	Player::CleanDefaultValues();
	std::cout << "Cleaning Gem default values...\n\n";
	Gem::CleanDefaultValues();

	std::cout << "Clearing musicMap...\n";
	musicMap.clear();
	std::cout << "Clearing chunkMap...\n";
	chunkMap.clear();
	std::cout << "Clearing textMap...\n";
	textMap.clear();
	std::cout << "Clearing playerMap...\n";
	playerMap.clear();
	std::cout << "Clearing gemGroup...\n";
	gemGroup.clear();
	std::cout << "Clearing asteroidGroup...\n";
	asteroidGroup.clear();
	std::cout << "Clearing IMGSpriteMap...\n\n";
	IMGSpriteMap.clear();

	std::cout << "Destroying renderer...\n";
	SDL_DestroyRenderer(Renderer);
	Renderer = nullptr;
	std::cout << "Destroying main window...\n";
	SDL_DestroyWindow(Window);
	Window = nullptr;

	std::cout << "Destroying keyboard...\n\n";
	Keyboard = nullptr;

	return;
}

void Game::Quit()
{
	std::cout << "\n------------- Cleanup -------------\n\n";
	Clean();

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
int Game::InitializeSystems()
{
	std::cout << "------------- Startup -------------\n\n";

	std::cout << "Initializing SDL...\n";
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		ShowErrorMessage();
		Quit();
		return -1;
	}
	std::cout << "Initializing IMG...\n";
	if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) != (IMG_INIT_JPG | IMG_INIT_PNG))
	{
		ShowErrorMessage();
		Quit();
		return -1;
	}
	std::cout << "Initializing TTF...\n";
	if (TTF_Init() < 0)
	{
		ShowErrorMessage();
		Quit();
		return -1;
	}
	std::cout << "Initializing Mixer...\n";
	if (Mix_Init(MIX_INIT_FLAC | MIX_INIT_MP3) != (MIX_INIT_FLAC | MIX_INIT_MP3))
	{
		ShowErrorMessage();
		Quit();
		return -1;
	}
	std::cout << "Opening audio device...\n\n";
	if (Mix_OpenAudio(48000, AUDIO_F32SYS, 2, 4096) < 0)
	{
		ShowErrorMessage();
		Quit();
		return -1;
	}

	return 0;
}

void Game::SetupGame()
{
	std::cout << "Importing settings...\n";
	ImportSettings();

	currentChunkVolume = Mix_MasterVolume(-1);
	lastChunkVolume = currentChunkVolume;

	startTimer.Duration = Config.COUNTDOWN_DURATION;
	startTimer.Interval = Config.COUNTDOWN_INTERVAL;

	std::cout << "Creating main window...\n";
	Window = SDL_CreateWindow("Space Shooter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Config.START_WINDOW_WIDTH, Config.START_WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
	std::cout << "Creating main renderer...\n\n";
	Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);

	std::cout << "Creating objects...\n";
	CreateObjects();

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
	boost::replace_all(value, "\\n", "\n");

	return std::pair<std::string, std::string>(key, value);
}

void Game::ImportSettings()
{
	std::unordered_map<std::string, int *> intMap;
	intMap["Window Width"] = &Config.START_WINDOW_WIDTH;
	intMap["Window Height"] = &Config.START_WINDOW_HEIGHT;

	intMap["Countdown Duration"] = &Config.COUNTDOWN_DURATION;
	intMap["Countdown Interval"] = &Config.COUNTDOWN_INTERVAL;

	intMap["Start Text Size"] = &Config.START_TEXT_SIZE;
	intMap["Score Text Size"] = &Config.SCORE_TEXT_SIZE;

	intMap["Player Frame Width"] = &Config.PLAYER_FRAME_WIDTH;
	intMap["Player Frame Height"] = &Config.PLAYER_FRAME_HEIGHT;
	intMap["Player IMG Frames"] = &Config.PLAYER_IMG_FRAMES;
	intMap["Player IMG Types"] = &Config.PLAYER_IMG_TYPES;
	intMap["Player Animations Per Second"] = &Config.PLAYER_ANIMATIONS_PER_SECOND;
	intMap["Player Effect Duration Seconds"] = &Config.PLAYER_EFFECT_DURATION_SECONDS;
	intMap["Player Rotation Speed"] = &Config.PLAYER_ROTATION_SPEED;

	intMap["Gem Frame Width"] = &Config.GEM_FRAME_WIDTH;
	intMap["Gem Frame Height"] = &Config.GEM_FRAME_HEIGHT;
	intMap["Gem IMG Frames"] = &Config.GEM_IMG_FRAMES;
	intMap["Gem IMG Types"] = &Config.GEM_IMG_TYPES;
	intMap["Gem Blink Factor"] = &Config.GEM_BLINK_FACTOR;
	intMap["Gem Blink Duration"] = &Config.GEM_BLINK_DURATION;
	intMap["Gem Lifetime Duration"] = &Config.GEM_LIFETIME_DURATION;
	intMap["Gem Maximum Brightness"] = &Config.GEM_MAXIMUM_BRIGHTNESS;
	intMap["Gem Minimum Brightness"] = &Config.GEM_MINIMUM_BRIGHTNESS;

	std::unordered_map<std::string, float *> floatMap;
	floatMap["Player Max Velocity Boost"] = &Config.PLAYER_MAX_VELOCITY_BOOST;
	floatMap["Player Acceleration Boost"] = &Config.PLAYER_ACCELEARION_BOOST;
	floatMap["Player Acceleration"] = &Config.PLAYER_ACCELEARION;
	floatMap["Player Max Velocity"] = &Config.PLAYER_MAX_VELOCITY;
	floatMap["Player Friction"] = &Config.PLAYER_FRICTION;
	floatMap["Player Scale"] = &Config.PLAYER_SCALE;
	floatMap["Player Radius"] = &Config.PLAYER_RADIUS;

	floatMap["Gem Scale"] = &Config.GEM_SCALE;
	floatMap["Gem Radius"] = &Config.GEM_RADIUS;

	std::unordered_map<std::string, std::string *> stringMap;
	stringMap["Font Path"] = &Config.FONT_PATH;

	std::unordered_map<std::string, std::wstring *> wstringMap;
	wstringMap["Start Text"] = &Config.START_TEXT;

	std::cout << "Opening settings file...\n";
	std::ifstream file("config/settings.txt", std::ios_base::in);
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
				int start_pos = (value[0] == '#' ? 1 : 0);
				Config.FONT_COLOR_HEX = std::stoi(value.substr(start_pos), nullptr, 16);
				continue;
			}

			auto itInt = intMap.find(key);
			if (itInt != intMap.end())
			{
				std::cout << "Initializing " << key << " ... \n";
				*(itInt->second) = std::stoi(value);
				continue;
			}

			auto itFloat = floatMap.find(key);
			if (itFloat != floatMap.end())
			{
				std::cout << "Initializing " << key << " ... \n";
				*(itFloat->second) = std::stof(value);
				continue;
			}

			auto itString = stringMap.find(key);
			if (itString != stringMap.end())
			{
				std::cout << "Initializing " << key << "...\n";
				*(itString->second) = value;
				continue;
			}

			auto itWString = wstringMap.find(key);
			if (itWString != wstringMap.end())
			{
				std::cout << "Initializing " << key << "...\n";
				*(itWString->second) = converter.from_bytes(value);
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

void Game::CreateObjects()
{
	IMGSpriteMap["startBackground"] = std::make_shared<IMGSprite>(Renderer, "assets/images/background_blurred.png", 1, 0);

	Uint8 r, g, b;
	r = ((Config.FONT_COLOR_HEX >> 16) & 0xFF);
	g = ((Config.FONT_COLOR_HEX >> 8) & 0xFF);
	b = (Config.FONT_COLOR_HEX & 0xFF);
	SDL_Color font_color = {r, g, b, 255};

	textMap["startText"] = std::make_shared<Text>(Renderer, Config.START_TEXT.c_str(), Config.FONT_PATH.c_str(), Config.START_TEXT_SIZE, font_color);
	SDL_FRect text_rect = textMap.at("startText")->GetRect();
	textMap.at("startText")->SetRectPos(static_cast<float>(screenWidth / 2) - text_rect.w / 2, static_cast<float>(screenHeight / 2) - text_rect.h / 2);

	IMGSpriteMap["gameBackground"] = std::make_shared<IMGSprite>(Renderer, "assets/images/background.png", 1, 0);

	asteroidGroup.push_back(std::make_shared<Asteroid>(Renderer, "assets/images/asteroid1.png", 1, 60, 200, screenWidth, screenHeight));

	std::cout << "Initializing Gem default values...\n";
	Gem::InitDefaultValues(Renderer, "assets/images/gems.png", Config.GEM_SCALE, Config.GEM_RADIUS, Config.GEM_FRAME_WIDTH, Config.GEM_FRAME_HEIGHT, Config.GEM_IMG_FRAMES, Config.GEM_IMG_TYPES, 0, Config.GEM_BLINK_DURATION, Config.GEM_LIFETIME_DURATION);
	gemGroup.push_back(Gem::NewGem(screenWidth, screenHeight));

	std::cout << "Initializing Player default values...\n\n";
	Player::InitDefaultValues(Renderer, Config.PLAYER_SCALE, Config.PLAYER_RADIUS, Config.PLAYER_FRAME_WIDTH, Config.PLAYER_FRAME_HEIGHT, Config.PLAYER_IMG_FRAMES, Config.PLAYER_IMG_TYPES, Config.PLAYER_ANIMATIONS_PER_SECOND, Config.PLAYER_ACCELEARION, Config.PLAYER_MAX_VELOCITY, Config.PLAYER_FRICTION, Config.PLAYER_EFFECT_DURATION_SECONDS, Config.PLAYER_ROTATION_SPEED);
	playerMap["Player1"] = std::make_shared<Player>("assets/images/player.png");
	SDL_Rect player_rect = playerMap.at("Player1")->GetIMGPartRect();
	playerMap.at("Player1")->SetRectPos(static_cast<float>(screenWidth / 2) - static_cast<float>(player_rect.w / 2), static_cast<float>(screenHeight / 2) - static_cast<float>(player_rect.h / 2));

	const wchar_t score_text[] = {L"0"};
	textMap["scoreText"] = std::make_shared<Text>(Renderer, score_text, Config.FONT_PATH.c_str(), Config.SCORE_TEXT_SIZE, font_color);
	textMap.at("scoreText")->SetRectPos(static_cast<float>(screenWidth) - textMap.at("scoreText")->GetRect().w, 0);

	chunkMap["startSound"] = std::make_shared<soundChunk>("assets/sounds/game_start.mp3");

	chunkMap["gemCollected"] = std::make_shared<soundChunk>("assets/sounds/gem_collected.mp3");

	chunkMap["gemMissed"] = std::make_shared<soundChunk>("assets/sounds/gem_missed.wav");

	musicMap["menuMusic"] = std::make_shared<soundMusic>("assets/sounds/menu_music.mp3");
	musicMap.at("menuMusic")->PlayMusic();

	musicMap["backgroundMusic"] = std::make_shared<soundMusic>("assets/sounds/background_music.mp3");

	Mix_VolumeMusic(Mix_MasterVolume(-1) / 5);

	return;
}

//* Game modes
void Game::GamePaused()
{
	SDL_RenderPresent(Renderer);

	return;
}

void Game::GameStarted()
{
	if (startSoundPlaying && !Mix_Playing(chunkMap.at("startSound")->GetChannel()))
	{
		musicMap.at("backgroundMusic")->PlayMusic();
		startSoundPlaying = false;
	}

	HandlePlayers();
	HandleGems();
	HandleAsteroids();
	CheckCollisions();
	animateUnorderedMap<std::string, std::shared_ptr<Player>>(&playerMap);

	SDL_RenderClear(Renderer);

	renderUnorderedMap<std::string, std::shared_ptr<IMGSprite>>(&IMGSpriteMap);
	renderVector<std::shared_ptr<Gem>>(&gemGroup);
	renderUnorderedMap<std::string, std::shared_ptr<Player>>(&playerMap);
	renderVector<std::shared_ptr<Asteroid>>(&asteroidGroup);
	renderUnorderedMap<std::string, std::shared_ptr<Text>>(&textMap);

	SDL_RenderPresent(Renderer);

	return;
}

void Game::GameTitleScreen()
{
	SDL_RenderClear(Renderer);

	IMGSpriteMap.at("startBackground")->Render();
	textMap.at("startText")->Render();

	SDL_RenderPresent(Renderer);

	if (!startCountdown)
	{
		return;
	}

	if (startTimer.Check(currentTicks))
	{
		Countdown -= startTimer.Interval;

		std::wstring countdown_str = std::to_wstring(Countdown);
		textMap.at("startText")->SetMessage(countdown_str.c_str());

		textMap.at("scoreText")->SetRectPos(static_cast<float>(screenWidth) - textMap.at("scoreText")->GetRect().w, 0);
	}
	if (Countdown <= 0)
	{
		startTimer.Stop();
		startCountdown = false;
		currentGameState = GAME_STARTED;

		IMGSpriteMap.erase("startBackground");
		textMap.erase("startText");

		for (std::shared_ptr<Gem> gem : gemGroup)
		{
			gem->SetTicks(currentTicks);
		}

		Mix_HaltMusic();
		chunkMap.at("startSound")->PlayChunk();
		startSoundPlaying = true;

		return;
	}

	return;
}

//* Game handling
void Game::HandlePlayers()
{
	for (const auto &[name, player] : playerMap)
	{
		player->CheckType(currentTicks);
		player->HandleKeys(screenWidth, screenHeight, gameClock.deltaTimeSeconds, Keyboard);
	}

	return;
}

void Game::HandleGems()
{
	for (const std::shared_ptr<Gem> &gem : gemGroup)
	{
		if (currentTicks >= gem->GetBlinkTicks())
		{
			gem->Blink(Config.GEM_MINIMUM_BRIGHTNESS, Config.GEM_MAXIMUM_BRIGHTNESS, Config.GEM_BLINK_FACTOR);
		}
		if (currentTicks >= gem->GetLifetimeTicks())
		{
			Score -= 1;
			chunkMap.at("gemMissed")->PlayChunk();

			std::wstring current_score = std::to_wstring(Score);
			textMap.at("scoreText")->SetMessage(current_score.c_str());

			textMap.at("scoreText")->SetRectPos(static_cast<float>(screenWidth) - textMap.at("scoreText")->GetRect().w, 0);

			gem->Randomize(screenWidth, screenHeight, currentTicks);
		}
	}

	return;
}

void Game::HandleAsteroids()
{
	for (const std::shared_ptr<Asteroid> &asteroid : asteroidGroup)
	{
		asteroid->Move(screenWidth, screenHeight, gameClock.deltaTimeSeconds);
	}

	return;
}

void Game::CheckCollisions()
{
	for (const auto &[name, player] : playerMap)
	{
		for (const std::shared_ptr<Gem> &gem : gemGroup)
		{
			if (!gem->Collideswith(player))
			{
				continue;
			}

			Score += 1;
			chunkMap.at("gemCollected")->PlayChunk();

			std::wstring current_score = std::to_wstring(Score);
			textMap.at("scoreText")->SetMessage(current_score.c_str());

			textMap.at("scoreText")->SetRectPos(static_cast<float>(screenWidth) - textMap.at("scoreText")->GetRect().w, 0);

			gem->Randomize(screenWidth, screenHeight, currentTicks);

			if (!player->GetGemCollected())
			{
				player->SetMaxVelocity(player->GetMaxVelocity() + Config.PLAYER_MAX_VELOCITY_BOOST);
				player->SetAcceleration(player->GetAcceleration() + Config.PLAYER_ACCELEARION_BOOST);
				player->SetGemCollected(true);
			}

			player->SetCollectionTicks(currentTicks);
		}

		for (const std::shared_ptr<Asteroid> &asteroid : asteroidGroup)
		{
			if (!asteroid->Collideswith(player))
			{
				continue;
			}

			Score -= 1;

			std::wstring current_score = std::to_wstring(Score);
			textMap.at("scoreText")->SetMessage(current_score.c_str());

			textMap.at("scoreText")->SetRectPos(static_cast<float>(screenWidth) - textMap.at("scoreText")->GetRect().w, 0);
		}
	}

	return;
}

//* Rendering
template <typename keyType, typename valueType>
void Game::renderUnorderedMap(std::unordered_map<keyType, valueType> *map)
{
	for (const auto &[name, element] : *map)
	{
		element->Render();
	}
}

template <typename elementType>
void Game::renderVector(std::vector<elementType> *vector)
{
	for (const auto &element : *vector)
	{
		element->Render();
	}
}

template <typename keyType, typename valueType>
void Game::animateUnorderedMap(std::unordered_map<keyType, valueType> *map)
{
	for (const auto &[name, element] : *map)
	{
		element->Animate(currentTicks);
	}
}