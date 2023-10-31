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
		currentTicks = GetCurrentTime();
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
	std::cout << "Clearing objectsGameOver...\n";
	objectsGameOver.ClearAll();
	std::cout << "Clearing objectsGamePaused...\n";
	objectsGamePaused.ClearAll();
	std::cout << "Clearing objectsGameRunning...\n";
	objectsGameRunning.ClearAll();
	std::cout << "Clearing objectsTitleScreen...\n";
	objectsTitleScreen.ClearAll();

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

	Score = Config.START_SCORE;

	currentGameState = TITLE_SCREEN;
	lastGameState = TITLE_SCREEN;

	gameEvents = EventHandler(this);
	startTimer = SecondTimer(Config.COUNTDOWN_DURATION_MILLISECONDS, Config.COUNTDOWN_INTERVAL_MILLISECONDS, this);

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
	//* Title screen

	objectsTitleScreen.IMGSprites["startBackground"] = std::make_shared<IMGSprite>(Renderer, "assets/images/background_blurred.png", 1, 0);

	objectsTitleScreen.Texts["startText"] = std::make_shared<Text>(Renderer, Config.START_TEXT, Config.FONT_PATH,
																   Config.START_TEXT_SIZE, Config.FONT_COLOR_HEX);
	SDL_FRect text_rect = objectsTitleScreen.Texts.at("startText")->GetRect();
	objectsTitleScreen.Texts.at("startText")->SetRectPos(static_cast<float>(screenWidth / 2) - text_rect.w / 2, static_cast<float>(screenHeight / 2) - text_rect.h / 2);

	objectsTitleScreen.Chunks["startSound"] = std::make_shared<soundChunk>("assets/sounds/game_start.mp3");

	objectsTitleScreen.Musics["menuMusic"] = std::make_shared<soundMusic>("assets/sounds/menu_music.mp3");
	objectsTitleScreen.Musics.at("menuMusic")->Play();

	//* Game running

	objectsGameRunning.IMGSprites["gameBackground"] = std::make_shared<IMGSprite>(Renderer, "assets/images/background.png", 1, 0);

	objectsGameRunning.Asteroids.push_back(std::make_shared<Asteroid>(Renderer, "assets/images/asteroid1.png", 1, 60, 200,
																	  screenWidth, screenHeight));

	objectsGameRunning.Gems.push_back(Gem::NewGem(Config, Renderer, "assets/images/gems.png", screenWidth, screenHeight));

	objectsGameRunning.Players["Player1"] = Player::NewPlayer(Config, Renderer, "assets/images/player.png");
	SDL_Rect player_rect = objectsGameRunning.Players.at("Player1")->GetIMGPartRect();
	objectsGameRunning.Players.at("Player1")->SetRectPos(static_cast<float>(screenWidth / 2) - static_cast<float>(player_rect.w / 2),
														 static_cast<float>(screenHeight / 2) - static_cast<float>(player_rect.h / 2));

	std::wstring current_score = std::to_wstring(Score);
	objectsGameRunning.Texts["scoreText"] = std::make_shared<Text>(Renderer, current_score, Config.FONT_PATH,
																   Config.SCORE_TEXT_SIZE, Config.FONT_COLOR_HEX);
	SDL_FRect score_rect = objectsGameRunning.Texts.at("scoreText")->GetRect();
	objectsGameRunning.Texts.at("scoreText")->SetRectPos(static_cast<float>(screenWidth) - score_rect.w, 0);

	objectsGameRunning.Chunks["gemCollected"] = std::make_shared<soundChunk>("assets/sounds/gem_collected.mp3");

	objectsGameRunning.Chunks["gemMissed"] = std::make_shared<soundChunk>("assets/sounds/gem_missed.wav");

	objectsGameRunning.Musics["backgroundMusic"] = std::make_shared<soundMusic>("assets/sounds/background_music.mp3");

	//* Game Paused

	objectsGamePaused.IMGSprites["pauseIcon"] = std::make_shared<IMGSprite>(Renderer, "assets/images/pause-button.png", 0.3, 0);

	return;
}

//* Game modes
void Game::GameTitleScreen()
{
	SDL_RenderClear(Renderer);

	RenderObjects(&objectsTitleScreen);

	SDL_RenderPresent(Renderer);

	return;
}

void Game::GameStarted()
{
	HandlePlayers();
	HandleGems();
	HandleAsteroids();
	CheckCollisions();

	AnimateObjects(&objectsGameRunning);

	SDL_RenderClear(Renderer);

	RenderObjects(&objectsGameRunning);

	SDL_RenderPresent(Renderer);

	return;
}

void Game::GamePaused()
{
	SDL_RenderClear(Renderer);

	switch (lastGameState)
	{
	case TITLE_SCREEN:
		RenderObjects(&objectsTitleScreen);
		break;
	case GAME_STARTED:
		RenderObjects(&objectsGameRunning);
		break;
	case GAME_OVER:
		RenderObjects(&objectsGameOver);
		break;
	default:
		break;
	};

	RenderObjects(&objectsGamePaused);

	SDL_RenderPresent(Renderer);

	return;
}

//* Game handling
Uint64 Game::GetCurrentTime()
{
	return SDL_GetTicks64() - totalTimePaused;
}

void Game::UpdateScore(int amount)
{
	if (amount < 0 && Score < static_cast<unsigned int>(abs(amount)))
	{
		currentGameState = GAME_OVER;
		return;
	}

	Score += amount;

	std::wstring current_score = std::to_wstring(Score);
	std::shared_ptr<Text> scoreText = objectsGameRunning.Texts.at("scoreText");

	scoreText->SetMessage(current_score);
	SDL_FRect score_rect = scoreText->GetRect();
	scoreText->SetRectPos(static_cast<float>(screenWidth) - score_rect.w, 0);

	return;
}

void Game::HandlePlayers()
{
	for (const auto &[name, player] : objectsGameRunning.Players)
	{
		player->UpdateGemCollected(currentTicks);
		player->HandleInput(screenWidth, screenHeight, gameClock.deltaTimeSeconds, Keyboard);
	}

	return;
}

void Game::HandleAsteroids()
{
	for (const std::shared_ptr<Asteroid> &asteroid : objectsGameRunning.Asteroids)
	{
		asteroid->Move(screenWidth, screenHeight, gameClock.deltaTimeSeconds);
	}

	return;
}

void Game::HandleGems()
{
	for (const std::shared_ptr<Gem> &gem : objectsGameRunning.Gems)
	{
		if (currentTicks >= gem->GetBlinkTicks())
		{
			gem->Blink(gameClock.deltaTimeSeconds);
		}
		if (currentTicks >= gem->GetLifetimeTicks())
		{
			UpdateScore(-1);
			objectsGameRunning.Chunks.at("gemMissed")->Play();
			gem->Randomize(screenWidth, screenHeight, currentTicks);
		}
	}

	return;
}

void Game::CheckCollisions()
{
	for (const auto &[name, player] : objectsGameRunning.Players)
	{
		for (const std::shared_ptr<Gem> &gem : objectsGameRunning.Gems)
		{
			if (!gem->Collideswith(player))
			{
				continue;
			}

			UpdateScore(1);
			objectsGameRunning.Chunks.at("gemCollected")->Play();

			gem->Randomize(screenWidth, screenHeight, currentTicks);

			if (!player->GetGemCollected())
			{
				player->SetMaxVelocity(player->GetMaxVelocity() + Config.PLAYER_MAX_VELOCITY_BOOST);
				player->SetAcceleration(player->GetAcceleration() + Config.PLAYER_ACCELEARION_BOOST);
				player->SetGemCollected(true);
			}

			player->SetCollectionTicks(currentTicks);
		}

		for (const std::shared_ptr<Asteroid> &asteroid : objectsGameRunning.Asteroids)
		{
			if (!asteroid->Collideswith(player))
			{
				continue;
			}

			UpdateScore(-1);
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

void Game::RenderObjects(objectStorage *storage)
{
	for (const auto &[name, element] : storage->IMGSprites)
	{
		element->Render();
	}
	for (const auto &element : storage->Asteroids)
	{
		element->Render();
	}
	for (const auto &element : storage->Gems)
	{
		element->Render();
	}
	for (const auto &[name, element] : storage->Players)
	{
		element->Render();
	}
	for (const auto &[name, element] : storage->Texts)
	{
		element->Render();
	}

	return;
}

void Game::AnimateObjects(objectStorage *storage)
{
	for (const auto &[name, element] : storage->Players)
	{
		element->Animate(currentTicks);
	}
	for (const auto &element : storage->Gems)
	{
		element->Animate(currentTicks);
	}

	return;
}