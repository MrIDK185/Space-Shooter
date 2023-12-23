#include "Game.hpp"
#include "Asteroid.hpp"
#include "Player.hpp"
#include "Gem.hpp"
#include "Text.hpp"

#include <iostream>
#include <boost/algorithm/string/replace.hpp>

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

static channelControl *channelControllerInstance = nullptr;

static void channelFinishedCallback(int channel)
{
	channelControllerInstance->channelFinished(channel);

	return;
}

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
		case TITLE_SCREEN:
			GameTitleScreen();
			break;

		case GAME_STARTED:
			GameStarted();
			break;

		case GAME_PAUSED:
			GamePaused();
			break;

		case GAME_OVER:
			GameOver();
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

	channelControllerInstance = &channelController;
	Mix_ChannelFinished(channelFinishedCallback);

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
	SpriteData sprite_data;
	TextData text_data;
	AsteroidData asteroid_data;
	AnimationData animation_data;
	GemData gem_data;
	PlayerData player_data;

	//* Title Screen

	sprite_data = {.destRenderer = Renderer,
				   .IMGPath = "assets/images/background_blurred.png",
				   .Scale = 1,
				   .Radius = 0};
	objectsTitleScreen.IMGSprites.emplace("startBackground",
										  IMGSprite(sprite_data));

	text_data = {.destRenderer = Renderer,
				 .Message = Config.START_TEXT,
				 .fontPath = Config.FONT_PATH,
				 .fontColor = Config.START_TEXT_COLOR,
				 .fontSize = Config.START_TEXT_SIZE};
	auto start_text = objectsTitleScreen.Texts.emplace("startText",
													   Text(text_data));
	start_text.first->second.SetRectPos(static_cast<float>(screenWidth / 2), static_cast<float>(screenHeight / 2));

	objectsTitleScreen.Chunks.emplace("startSound",
									  soundChunk("assets/sounds/game_start.mp3", &channelController));

	auto menu_music = objectsTitleScreen.Musics.emplace("menuMusic",
														soundMusic("assets/sounds/menu_music.mp3"));
	menu_music.first->second.Play();

	//* Game running

	sprite_data = {.destRenderer = Renderer,
				   .IMGPath = "assets/images/background.png",
				   .Scale = 1,
				   .Radius = 0};
	objectsGameRunning.IMGSprites.emplace("gameBackground",
										  IMGSprite(sprite_data));

	sprite_data = {.destRenderer = Renderer,
				   .IMGPath = "assets/images/asteroid1.png",
				   .Scale = 1,
				   .Radius = 60};
	asteroid_data = {.Velocity = 200};
	objectsGameRunning.Asteroids.emplace_back(Asteroid(sprite_data, asteroid_data));

	sprite_data = {.destRenderer = Renderer,
				   .IMGPath = "assets/images/gems.png",
				   .Scale = Config.GEM_SCALE,
				   .Radius = Config.GEM_RADIUS};
	animation_data = {.FRAME_WIDTH = Config.GEM_FRAME_WIDTH,
					  .FRAME_HEIGHT = Config.GEM_FRAME_HEIGHT,
					  .IMG_FRAMES = Config.GEM_IMG_FRAMES,
					  .IMG_TYPES = Config.GEM_IMG_TYPES,
					  .ANIMATIONS_PER_SECOND = 0};
	gem_data = {.blinkDuration = Config.GEM_BLINK_DURATION,
				.lifetimeDuration = Config.GEM_LIFETIME_DURATION,
				.minimumBrightness = Config.GEM_MINIMUM_BRIGHTNESS,
				.maximumBrightness = Config.GEM_MAXIMUM_BRIGHTNESS,
				.blinkFactor = Config.GEM_BLINK_FACTOR};
	objectsGameRunning.Gems.emplace_back(Gem(sprite_data, animation_data, gem_data));

	sprite_data = {.destRenderer = Renderer,
				   .IMGPath = "assets/images/player.png",
				   .Scale = Config.PLAYER_SCALE,
				   .Radius = Config.PLAYER_RADIUS};
	animation_data = {.FRAME_WIDTH = Config.PLAYER_FRAME_WIDTH,
					  .FRAME_HEIGHT = Config.PLAYER_FRAME_HEIGHT,
					  .IMG_FRAMES = Config.PLAYER_IMG_FRAMES,
					  .IMG_TYPES = Config.PLAYER_IMG_TYPES,
					  .ANIMATIONS_PER_SECOND = Config.PLAYER_ANIMATIONS_PER_SECOND};
	player_data = {.Acceleration = Config.PLAYER_ACCELEARION,
				   .maxVelocity = Config.PLAYER_MAX_VELOCITY,
				   .Friction = Config.PLAYER_FRICTION,
				   .effectDuration = Config.PLAYER_EFFECT_DURATION_SECONDS,
				   .rotationSpeed = Config.PLAYER_ROTATION_SPEED};
	auto player = objectsGameRunning.Players.emplace("Player1",
													 Player(sprite_data, animation_data, player_data));
	player.first->second.SetRectPos(static_cast<float>(screenWidth / 2),
									static_cast<float>(screenHeight / 2));

	std::wstring current_score = std::to_wstring(Score);
	text_data = {.destRenderer = Renderer,
				 .Message = current_score,
				 .fontPath = Config.FONT_PATH,
				 .fontColor = Config.INGAME_TEXT_COLOR,
				 .fontSize = Config.INGAME_TEXT_SIZE};
	auto score_text = objectsGameRunning.Texts.emplace("scoreText",
													   Text(text_data));
	score_text.first->second.SetRectPos(static_cast<float>(screenWidth), 0, NE);

	objectsGameRunning.Chunks.emplace("gemCollected",
									  soundChunk("assets/sounds/gem_collected.mp3", &channelController));

	objectsGameRunning.Chunks.emplace("gemMissed",
									  soundChunk("assets/sounds/gem_missed.wav", &channelController));

	objectsGameRunning.Musics.emplace("backgroundMusic",
									  soundMusic("assets/sounds/background_music.mp3"));

	//* Game Paused

	sprite_data = {.destRenderer = Renderer,
				   .IMGPath = "assets/images/pause-button.png",
				   .Scale = 0.3,
				   .Radius = 0};
	objectsGamePaused.IMGSprites.emplace("pauseIcon",
										 IMGSprite(sprite_data));

	sprite_data = {.destRenderer = Renderer,
				   .IMGPath = "assets/images/Overlay.png",
				   .Scale = 1,
				   .Radius = 0};
	objectsGamePaused.IMGSprites.emplace("darkenOverlay",
										 IMGSprite(sprite_data));

	//* Game Over

	sprite_data = {.destRenderer = Renderer,
				   .IMGPath = "assets/images/background_blurred.png",
				   .Scale = 1,
				   .Radius = 0};
	objectsGameOver.IMGSprites.emplace("endBackground",
									   IMGSprite(sprite_data));

	text_data = {.destRenderer = Renderer,
				 .Message = Config.GAME_OVER_TEXT,
				 .fontPath = Config.FONT_PATH,
				 .fontColor = Config.GAME_OVER_TEXT_COLOR,
				 .fontSize = Config.GAME_OVER_TEXT_SIZE};
	auto game_over = objectsGameOver.Texts.emplace("gameOver",
												   Text(text_data));
	game_over.first->second.SetRectPos(static_cast<float>(screenWidth / 2), static_cast<float>(screenHeight / 2));

	return;
}

void Game::Reset()
{
	currentGameState = TITLE_SCREEN;
	lastGameState = TITLE_SCREEN;

	startTimer.Reset();

	Score = Config.START_SCORE;
	std::wstring current_score = std::to_wstring(Score);
	objectsGameRunning.Texts.at("scoreText").SetMessage(current_score);
	objectsGameRunning.Texts.at("scoreText").SetRectPos(static_cast<float>(screenWidth), 0, NE);

	objectsTitleScreen.Texts.at("startText").SetMessage(Config.START_TEXT);
	objectsTitleScreen.Texts.at("startText").SetRectPos(static_cast<float>(screenWidth / 2), static_cast<float>(screenHeight / 2));

	for (auto &[name, player] : objectsGameRunning.Players)
	{
		player.SetRectPos(static_cast<float>(screenWidth / 2),
						  static_cast<float>(screenHeight / 2));
		player.Reset(Config.PLAYER_ACCELEARION, Config.PLAYER_MAX_VELOCITY, Config.PLAYER_FRICTION);
	}

	for (auto &gem : objectsGameRunning.Gems)
	{
		gem.Randomize(screenWidth, screenHeight, GetCurrentTime());
	}

	for (auto &asteroid : objectsGameRunning.Asteroids)
	{
		asteroid.Randomize(screenWidth, screenHeight);
	}

	objectsGameRunning.Musics.at("backgroundMusic").Stop();
	objectsTitleScreen.Musics.at("menuMusic").Play();

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

void Game::GameOver()
{
	SDL_RenderClear(Renderer);

	RenderObjects(&objectsGameOver);

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
		HandleGameOver();
		return;
	}

	Score += amount;

	std::wstring current_score = std::to_wstring(Score);
	Text &scoreText = objectsGameRunning.Texts.at("scoreText");

	scoreText.SetMessage(current_score);
	scoreText.SetRectPos(static_cast<float>(screenWidth), 0, NE);

	return;
}

void Game::HandlePlayers()
{
	for (auto &[name, player] : objectsGameRunning.Players)
	{
		player.UpdateGemCollected(currentTicks);
		player.HandleInput(screenWidth, screenHeight, gameClock.deltaTimeSeconds, Keyboard);
	}

	return;
}

void Game::HandleAsteroids()
{
	for (auto &asteroid : objectsGameRunning.Asteroids)
	{
		asteroid.Move(screenWidth, screenHeight, gameClock.deltaTimeSeconds);
	}

	return;
}

void Game::HandleGems()
{
	for (auto &gem : objectsGameRunning.Gems)
	{
		if (currentTicks >= gem.GetBlinkTicks())
		{
			gem.Blink(gameClock.deltaTimeSeconds);
		}
		if (currentTicks >= gem.GetLifetimeTicks())
		{
			UpdateScore(-1);
			objectsGameRunning.Chunks.at("gemMissed").Play();
			gem.Randomize(screenWidth, screenHeight, currentTicks);
		}
	}

	return;
}

void Game::CheckCollisions()
{
	for (auto &[name, player] : objectsGameRunning.Players)
	{
		for (auto &gem : objectsGameRunning.Gems)
		{
			if (!gem.Collideswith(player))
			{
				continue;
			}

			UpdateScore(1);
			objectsGameRunning.Chunks.at("gemCollected").Play();

			gem.Randomize(screenWidth, screenHeight, currentTicks);

			if (!player.GetGemCollected())
			{
				player.SetMaxVelocity(player.GetMaxVelocity() + Config.PLAYER_MAX_VELOCITY_BOOST);
				player.SetAcceleration(player.GetAcceleration() + Config.PLAYER_ACCELEARION_BOOST);
				player.SetGemCollected(true);
			}

			player.SetCollectionTicks(currentTicks);
		}

		for (const auto &asteroid : objectsGameRunning.Asteroids)
		{
			if (!asteroid.Collideswith(player))
			{
				continue;
			}

			HandleGameOver();
		}
	}

	return;
}

void Game::HandleGameOver()
{
	currentGameState = GAME_OVER;

	std::wstring current_score = std::to_wstring(Score);
	std::wstring high_score = std::to_wstring(Highscore);
	if (Score > Highscore)
	{
		Highscore = Score;
		high_score = current_score;
	}

	Text &game_over = objectsGameOver.Texts.at("gameOver");
	std::wstring game_over_text = Config.GAME_OVER_TEXT;
	boost::algorithm::replace_all(game_over_text, L"{S}", current_score);
	boost::algorithm::replace_all(game_over_text, L"{HS}", high_score);
	game_over.SetMessage(game_over_text);
	game_over.SetRectPos(static_cast<float>(screenWidth / 2), static_cast<float>(screenHeight / 2));

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
		element.Render();
	}
	for (const auto &element : storage->Asteroids)
	{
		element.Render();
	}
	for (const auto &element : storage->Gems)
	{
		element.Render();
	}
	for (const auto &[name, element] : storage->Players)
	{
		element.Render();
	}
	for (const auto &[name, element] : storage->Texts)
	{
		element.Render();
	}

	return;
}

void Game::AnimateObjects(objectStorage *storage)
{
	for (auto &[name, element] : storage->Players)
	{
		element.Animate(currentTicks);
	}
	for (auto &element : storage->Gems)
	{
		element.Animate(currentTicks);
	}

	return;
}