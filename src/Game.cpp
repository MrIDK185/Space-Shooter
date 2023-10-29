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

int Game::Run()
{
	SetupGame();

	std::cout << "------- Startup successful --------\n\n";

	while (Running)
	{
		gameClock.Tick();
		currentTicks = SDL_GetTicks64();
		Keyboard = SDL_GetKeyboardState(nullptr);

		gameEvents.HandleEvents();

		UpdateRenderScale();

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

	Cleanup();
	return 0;
}

//* Cleanup
void Game::Cleanup()
{
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

void Game::SetupGame()
{
	std::cout << "Importing settings...\n";
	ImportSettings();

	SDL_DisplayMode display_mode;
	SDL_GetCurrentDisplayMode(0, &display_mode);
	screenWidth = display_mode.w;
	screenHeight = display_mode.h;

	gameEvents = EventHandler(this);
	startTimer = SecondTimer(Config.COUNTDOWN_DURATION_MILLISECONDS, Config.COUNTDOWN_INTERVAL_MILLISECONDS, TimerCallback);

	volumeController.changeMasterVolume(MIX_MAX_VOLUME);

	std::cout << "Creating main window...\n";
	Window = SDL_CreateWindow("Space Shooter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Config.START_WINDOW_WIDTH, Config.START_WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
	std::cout << "Creating main renderer...\n\n";
	Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	std::cout << "Creating objects...\n";
	CreateObjects();

	return;
}

std::pair<std::string, std::string> Game::SplitKeyValue(const std::string &string)
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

void Game::ApplySetting(std::unordered_map<std::string, unsigned int *> *uint_map,
						std::unordered_map<std::string, float *> *float_map,
						std::unordered_map<std::string, std::string *> *string_map,
						std::unordered_map<std::string, std::wstring *> *wstring_map,
						const std::string &key, const std::string &value)
{
	if (key == "Font Color")
	{
		std::cout << "Initializing " << key << "...\n";
		int start_pos = (value[0] == '#' ? 1 : 0);
		Config.FONT_COLOR_HEX = std::stoi(value.substr(start_pos), nullptr, 16);
		return;
	}

	auto itUint = uint_map->find(key);
	if (itUint != uint_map->end())
	{
		if (value.find('-') != std::string::npos)
		{
			return;
		}

		std::cout << "Initializing " << key << " ... \n";
		*(itUint->second) = static_cast<unsigned int>(std::stoul(value));
		return;
	}

	auto itFloat = float_map->find(key);
	if (itFloat != float_map->end())
	{
		std::cout << "Initializing " << key << " ... \n";
		*(itFloat->second) = std::stof(value);
		return;
	}

	auto itString = string_map->find(key);
	if (itString != string_map->end())
	{
		std::cout << "Initializing " << key << "...\n";
		*(itString->second) = value;
		return;
	}

	auto itWString = wstring_map->find(key);
	if (itWString != wstring_map->end())
	{
		std::cout << "Initializing " << key << "...\n";
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		*(itWString->second) = converter.from_bytes(value);
		return;
	}
}

void Game::ImportSettings()
{
	std::unordered_map<std::string, unsigned int *> uintMap;
	uintMap["Window Width"] = &Config.START_WINDOW_WIDTH;
	uintMap["Window Height"] = &Config.START_WINDOW_HEIGHT;

	uintMap["Countdown Duration Milliseconds"] = &Config.COUNTDOWN_DURATION_MILLISECONDS;
	uintMap["Countdown Interval Milliseconds"] = &Config.COUNTDOWN_INTERVAL_MILLISECONDS;

	uintMap["Start Text Size"] = &Config.START_TEXT_SIZE;
	uintMap["Score Text Size"] = &Config.SCORE_TEXT_SIZE;

	uintMap["Player Frame Width"] = &Config.PLAYER_FRAME_WIDTH;
	uintMap["Player Frame Height"] = &Config.PLAYER_FRAME_HEIGHT;
	uintMap["Player IMG Frames"] = &Config.PLAYER_IMG_FRAMES;
	uintMap["Player IMG Types"] = &Config.PLAYER_IMG_TYPES;
	uintMap["Player Animations Per Second"] = &Config.PLAYER_ANIMATIONS_PER_SECOND;
	uintMap["Player Effect Duration Seconds"] = &Config.PLAYER_EFFECT_DURATION_SECONDS;
	uintMap["Player Rotation Speed"] = &Config.PLAYER_ROTATION_SPEED;

	uintMap["Gem Frame Width"] = &Config.GEM_FRAME_WIDTH;
	uintMap["Gem Frame Height"] = &Config.GEM_FRAME_HEIGHT;
	uintMap["Gem IMG Frames"] = &Config.GEM_IMG_FRAMES;
	uintMap["Gem IMG Types"] = &Config.GEM_IMG_TYPES;
	uintMap["Gem Blink Factor"] = &Config.GEM_BLINK_FACTOR;
	uintMap["Gem Blink Duration"] = &Config.GEM_BLINK_DURATION;
	uintMap["Gem Lifetime Duration"] = &Config.GEM_LIFETIME_DURATION;
	uintMap["Gem Maximum Brightness"] = &Config.GEM_MAXIMUM_BRIGHTNESS;
	uintMap["Gem Minimum Brightness"] = &Config.GEM_MINIMUM_BRIGHTNESS;

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

	if (!file)
	{
		std::cout << "Closing settings file...\n\n";
		file.close();

		uintMap.clear();
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
			ApplySetting(&uintMap, &floatMap, &stringMap, &wstringMap, key, value);
		}

		catch (const std::invalid_argument &error)
		{
			std::cerr << "An error occurred while trying to initialize a setting: " << error.what() << "\n";
			continue;
		}
		catch (const std::out_of_range &error)
		{
			std::cerr << "An error occurred while trying to initialize a setting: " << error.what() << "\n";
			continue;
		}
	}

	std::cout << "Closing settings file...\n\n";
	file.close();

	uintMap.clear();
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

	textMap["startText"] = std::make_shared<Text>(Renderer, Config.START_TEXT, Config.FONT_PATH, Config.START_TEXT_SIZE, font_color);
	SDL_FRect text_rect = textMap.at("startText")->GetRect();
	textMap.at("startText")->SetRectPos(static_cast<float>(screenWidth / 2) - text_rect.w / 2, static_cast<float>(screenHeight / 2) - text_rect.h / 2);

	IMGSpriteMap["gameBackground"] = std::make_shared<IMGSprite>(Renderer, "assets/images/background.png", 1, 0);

	asteroidGroup.push_back(std::make_shared<Asteroid>(Renderer, "assets/images/asteroid1.png", 1, 60, 200, screenWidth, screenHeight));

	gemGroup.push_back(Gem::NewGem(Config, Renderer, "assets/images/gems.png", screenWidth, screenHeight));

	playerMap["Player1"] = Player::NewPlayer(Config, Renderer, "assets/images/player.png");
	SDL_Rect player_rect = playerMap.at("Player1")->GetIMGPartRect();
	playerMap.at("Player1")->SetRectPos(static_cast<float>(screenWidth / 2) - static_cast<float>(player_rect.w / 2), static_cast<float>(screenHeight / 2) - static_cast<float>(player_rect.h / 2));

	const wchar_t score_text[] = {L"0"};
	textMap["scoreText"] = std::make_shared<Text>(Renderer, score_text, Config.FONT_PATH, Config.SCORE_TEXT_SIZE, font_color);
	textMap.at("scoreText")->SetRectPos(static_cast<float>(screenWidth) - textMap.at("scoreText")->GetRect().w, 0);

	chunkMap["startSound"] = std::make_shared<soundChunk>("assets/sounds/game_start.mp3");

	chunkMap["gemCollected"] = std::make_shared<soundChunk>("assets/sounds/gem_collected.mp3");

	chunkMap["gemMissed"] = std::make_shared<soundChunk>("assets/sounds/gem_missed.wav");

	musicMap["menuMusic"] = std::make_shared<soundMusic>("assets/sounds/menu_music.mp3");
	musicMap.at("menuMusic")->Play();

	musicMap["backgroundMusic"] = std::make_shared<soundMusic>("assets/sounds/background_music.mp3");

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
	HandlePlayers();
	HandleGems();
	HandleAsteroids();
	CheckCollisions();
	AnimateUnorderedMap<std::string, std::shared_ptr<Player>>(&playerMap);

	SDL_RenderClear(Renderer);

	RenderUnorderedMap<std::string, std::shared_ptr<IMGSprite>>(&IMGSpriteMap);
	RenderVector<std::shared_ptr<Gem>>(&gemGroup);
	RenderUnorderedMap<std::string, std::shared_ptr<Player>>(&playerMap);
	RenderVector<std::shared_ptr<Asteroid>>(&asteroidGroup);
	RenderUnorderedMap<std::string, std::shared_ptr<Text>>(&textMap);

	SDL_RenderPresent(Renderer);

	return;
}

void Game::GameTitleScreen()
{
	SDL_RenderClear(Renderer);

	IMGSpriteMap.at("startBackground")->Render();
	textMap.at("startText")->Render();

	SDL_RenderPresent(Renderer);

	return;
}

//* Game handling
void Game::HandlePlayers()
{
	for (const auto &[name, player] : playerMap)
	{
		player->UpdateGemCollected(currentTicks);
		player->HandleInput(screenWidth, screenHeight, gameClock.deltaTimeSeconds, Keyboard);
	}

	return;
}

void Game::HandleGems()
{
	for (const std::shared_ptr<Gem> &gem : gemGroup)
	{
		if (currentTicks >= gem->GetBlinkTicks())
		{
			gem->Blink(gameClock.deltaTimeSeconds);
		}
		if (currentTicks >= gem->GetLifetimeTicks())
		{
			Score -= 1;
			chunkMap.at("gemMissed")->Stop();
			chunkMap.at("gemMissed")->Play();

			std::wstring current_score = std::to_wstring(Score);
			textMap.at("scoreText")->SetMessage(current_score);

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
			chunkMap.at("gemCollected")->Stop();
			chunkMap.at("gemCollected")->Play();

			std::wstring current_score = std::to_wstring(Score);
			textMap.at("scoreText")->SetMessage(current_score);

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
			textMap.at("scoreText")->SetMessage(current_score);

			textMap.at("scoreText")->SetRectPos(static_cast<float>(screenWidth) - textMap.at("scoreText")->GetRect().w, 0);
		}
	}

	return;
}

//* Rendering
void Game::UpdateRenderScale()
{
	int current_window_width, current_window_height;
	SDL_GetWindowSizeInPixels(Window, &current_window_width, &current_window_height);
	float window_width_scale = static_cast<float>(current_window_width) / static_cast<float>(screenWidth);
	float window_height_scale = static_cast<float>(current_window_height) / static_cast<float>(screenHeight);

	SDL_RenderSetScale(Renderer, window_width_scale, window_height_scale);

	return;
}

template <typename keyType, typename valueType>
void Game::RenderUnorderedMap(std::unordered_map<keyType, valueType> *map)
{
	for (const auto &[name, element] : *map)
	{
		element->Render();
	}

	return;
}

template <typename elementType>
void Game::RenderVector(std::vector<elementType> *vector)
{
	for (const auto &element : *vector)
	{
		element->Render();
	}

	return;
}

template <typename keyType, typename valueType>
void Game::AnimateUnorderedMap(std::unordered_map<keyType, valueType> *map)
{
	for (const auto &[name, element] : *map)
	{
		element->Animate(currentTicks);
	}

	return;
}