#include "Game.hpp"
#include "Asteroid.hpp"
#include "Player.hpp"
#include "Gem.hpp"
#include "Text.hpp"
#include "Sound.hpp"

#include <iostream>

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
	Config.ImportSettings();

	gameEvents = EventHandler(this);
	startTimer = SecondTimer(Config.COUNTDOWN_DURATION_MILLISECONDS, Config.COUNTDOWN_INTERVAL_MILLISECONDS, TimerCallback);

	SDL_DisplayMode display_mode;
	SDL_GetCurrentDisplayMode(0, &display_mode);
	screenWidth = display_mode.w;
	screenHeight = display_mode.h;

	volumeController.changeMasterVolume(MIX_MAX_VOLUME);

	std::cout << "Creating main window...\n";
	Window = SDL_CreateWindow("Space Shooter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Config.START_WINDOW_WIDTH, Config.START_WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
	std::cout << "Creating main renderer...\n\n";
	Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	std::cout << "Creating objects...\n";
	CreateObjects();

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