#include "Game/Game.hpp"
#include "Game/EventCode.hpp"
#include "Game/EventType.hpp"
#include "Sprite/Asteroid.hpp"
#include "Sprite/Player.hpp"
#include "Sprite/Gem.hpp"
#include "Sprite/Text.hpp"

#include <iostream>
#include <boost/algorithm/string/replace.hpp>

namespace
{
	channelControl *channelControllerInstance = nullptr;

	void channelFinishedCallback(int channel)
	{
		channelControllerInstance->channelFinished(channel);
		return;
	}

	void musicFinishedCallback()
	{
		channelControllerInstance->musicFinished();
		return;
	}
}

void Game::Run()
{
	SetupGame();

	std::cout << "\n------- Startup successful --------\n\n";

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
	return;
}

//* Cleanup
void Game::Cleanup()
{
	std::cout << "\n------------- Cleanup -------------\n\n";

	std::cout << "Clearing objectsGameOver...\n";
	std::cout << "Clearing objectsGamePaused...\n";
	std::cout << "Clearing objects...\n";
	std::cout << "Clearing objectsTitleScreen...\n";
	objects.ClearAll();

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

	SDL_RegisterEvents(NUM_EVENTS);
	gameEvents = EventHandler(this);
	startTimer = Timer(COUNTDOWN_DECREMENT, GAME_START,
					   Config.COUNTDOWN_DURATION_MILLISECONDS, Config.COUNTDOWN_INTERVAL_MILLISECONDS,
					   &currentGameState);

	channelControllerInstance = &channelController;
	Mix_ChannelFinished(channelFinishedCallback);
	Mix_HookMusicFinished(musicFinishedCallback);

	Mix_AllocateChannels(MIX_CHANNELS);
	Mix_ReserveChannels(MIX_CHANNELS);

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
	InitializeDefaultObjectData();
	CreateObjects();

	return;
}

void Game::InitializeDefaultObjectData()
{
	asteroidDataDefault.first = {.destRenderer = Renderer,
								 .IMGPath = "assets/images/asteroid1.png",
								 .Scale = 1,
								 .Radius = 60};
	asteroidDataDefault.second = {.Velocity = 200};

	gemDataDefault.first = {.FRAME_WIDTH = Config.GEM_FRAME_WIDTH,
							.FRAME_HEIGHT = Config.GEM_FRAME_HEIGHT,
							.IMG_FRAMES = Config.GEM_IMG_FRAMES,
							.IMG_TYPES = Config.GEM_IMG_TYPES,
							.ANIMATIONS_PER_SECOND = 0};
	gemDataDefault.second = {.blinkDurationMilliseconds = Config.GEM_BLINK_DURATION_MILLISECONDS,
							 .lifetimeDurationMilliseconds = Config.GEM_LIFETIME_DURATION_MILLISECONDS,
							 .minimumBrightness = Config.GEM_MINIMUM_BRIGHTNESS,
							 .maximumBrightness = Config.GEM_MAXIMUM_BRIGHTNESS,
							 .blinkFactor = Config.GEM_BLINK_FACTOR,
							 .game_state = &currentGameState};

	playerDataDefault.first = {.FRAME_WIDTH = Config.PLAYER_FRAME_WIDTH,
							   .FRAME_HEIGHT = Config.PLAYER_FRAME_HEIGHT,
							   .IMG_FRAMES = Config.PLAYER_IMG_FRAMES,
							   .IMG_TYPES = Config.PLAYER_IMG_TYPES,
							   .ANIMATIONS_PER_SECOND = Config.PLAYER_ANIMATIONS_PER_SECOND};
	playerDataDefault.second = {.Acceleration = Config.PLAYER_ACCELEARION,
								.maxVelocity = Config.PLAYER_MAX_VELOCITY,
								.Friction = Config.PLAYER_FRICTION,
								.effectDurationMilliseconds = Config.PLAYER_EFFECT_DURATION_MILLISECONDS,
								.rotationSpeed = Config.PLAYER_ROTATION_SPEED,
								.gameState = &currentGameState};

	return;
}

void Game::CreateObjectsTitleScreen()
{
	std::vector<Text> &Texts = objects.gameModes[TITLE_SCREEN].first;
	std::vector<IMGSprite> &IMGSprites = objects.gameModes[TITLE_SCREEN].second;

	SpriteData sprite_data;
	TextData text_data;

	sprite_data = {.destRenderer = Renderer,
				   .IMGPath = "assets/images/background_blurred.png",
				   .Scale = 1,
				   .Radius = 0};
	IMGSprites.emplace_back(IMGSprite(sprite_data));

	text_data = {.destRenderer = Renderer,
				 .Message = Config.START_TEXT,
				 .fontPath = Config.FONT_PATH,
				 .fontColor = Config.START_TEXT_COLOR,
				 .fontSize = Config.START_TEXT_SIZE};
	Text &start_text = Texts.emplace_back(Text(text_data));
	objects.SetText("startText", start_text);
	start_text.SetRectPos(static_cast<float>(screenWidth / 2), static_cast<float>(screenHeight / 2));

	objects.Chunks.emplace("startSound",
						   soundChunk("assets/sounds/game_start.mp3", &channelController));

	auto menu_music = objects.Musics.emplace("menuMusic",
											 soundMusic("assets/sounds/menu_music.mp3", &channelController));
	menu_music.first->second.Play();

	return;
}

void Game::CreateObjectsGameRunning()
{
	std::vector<Text> &Texts = objects.gameModes[GAME_STARTED].first;
	std::vector<IMGSprite> &IMGSprites = objects.gameModes[GAME_STARTED].second;

	SpriteData sprite_data;
	TextData text_data;

	sprite_data = {.destRenderer = Renderer,
				   .IMGPath = "assets/images/background.png",
				   .Scale = 1,
				   .Radius = 0};
	IMGSprites.emplace_back(IMGSprite(sprite_data));

	objects.Asteroids.emplace_back(Asteroid(asteroidDataDefault.first, asteroidDataDefault.second));

	sprite_data = {.destRenderer = Renderer,
				   .IMGPath = "assets/images/gems.png",
				   .Scale = Config.GEM_SCALE,
				   .Radius = Config.GEM_RADIUS};
	Gem &gem = objects.Gems.emplace_back(Gem(sprite_data, gemDataDefault.first, gemDataDefault.second));
	gem.Randomize(screenWidth, screenHeight);

	sprite_data = {.destRenderer = Renderer,
				   .IMGPath = "assets/images/player.png",
				   .Scale = Config.PLAYER_SCALE,
				   .Radius = Config.PLAYER_RADIUS};
	objects.Player = Player(sprite_data, playerDataDefault.first, playerDataDefault.second);
	objects.Player.SetRectPos(static_cast<float>(screenWidth / 2),
							  static_cast<float>(screenHeight / 2));

	std::wstring current_score = std::to_wstring(Score);
	text_data = {.destRenderer = Renderer,
				 .Message = current_score,
				 .fontPath = Config.FONT_PATH,
				 .fontColor = Config.INGAME_TEXT_COLOR,
				 .fontSize = Config.INGAME_TEXT_SIZE};
	Text &score_text = Texts.emplace_back(Text(text_data));
	objects.SetText("scoreText", score_text);
	score_text.SetRectPos(static_cast<float>(screenWidth), 0, NE);

	objects.Chunks.emplace("gemCollected",
						   soundChunk("assets/sounds/gem_collected.mp3", &channelController));

	objects.Chunks.emplace("gemMissed",
						   soundChunk("assets/sounds/gem_missed.wav", &channelController));

	objects.Musics.emplace("backgroundMusic",
						   soundMusic("assets/sounds/background_music.mp3", &channelController));

	return;
}

void Game::CreateObjectsGamePaused()
{
	std::vector<IMGSprite> &IMGSprites = objects.gameModes[GAME_PAUSED].second;

	SpriteData sprite_data;

	sprite_data = {.destRenderer = Renderer,
				   .IMGPath = "assets/images/pause-button.png",
				   .Scale = 0.3,
				   .Radius = 0};
	IMGSprites.emplace_back(IMGSprite(sprite_data));

	sprite_data = {.destRenderer = Renderer,
				   .IMGPath = "assets/images/Overlay.png",
				   .Scale = 1,
				   .Radius = 0};
	IMGSprites.emplace_back(IMGSprite(sprite_data));

	return;
}

void Game::CreateObjectsGameOver()
{
	std::vector<Text> &Texts = objects.gameModes[GAME_OVER].first;
	std::vector<IMGSprite> &IMGSprites = objects.gameModes[GAME_OVER].second;

	SpriteData sprite_data;
	TextData text_data;

	sprite_data = {.destRenderer = Renderer,
				   .IMGPath = "assets/images/background_blurred.png",
				   .Scale = 1,
				   .Radius = 0};
	IMGSprites.emplace_back(IMGSprite(sprite_data));

	text_data = {.destRenderer = Renderer,
				 .Message = Config.GAME_OVER_TEXT,
				 .fontPath = Config.FONT_PATH,
				 .fontColor = Config.GAME_OVER_TEXT_COLOR,
				 .fontSize = Config.GAME_OVER_TEXT_SIZE};
	Text &game_over = Texts.emplace_back(Text(text_data));
	objects.SetText("gameOver", game_over);
	game_over.SetRectPos(static_cast<float>(screenWidth / 2), static_cast<float>(screenHeight / 2));

	return;
}

void Game::CreateObjects()
{
	CreateObjectsTitleScreen();
	CreateObjectsGameRunning();
	CreateObjectsGamePaused();
	CreateObjectsGameOver();

	return;
}

void Game::Reset()
{
	currentGameState = TITLE_SCREEN;
	lastGameState = TITLE_SCREEN;

	Score = Config.START_SCORE;
	std::wstring current_score = std::to_wstring(Score);
	objects.GetText("scoreText").SetMessage(current_score);
	objects.GetText("scoreText").SetRectPos(static_cast<float>(screenWidth), 0, NE);

	objects.GetText("startText").SetMessage(Config.START_TEXT);
	objects.GetText("startText").SetRectPos(static_cast<float>(screenWidth / 2), static_cast<float>(screenHeight / 2));

	objects.Player.SetRectPos(static_cast<float>(screenWidth / 2),
							  static_cast<float>(screenHeight / 2));
	objects.Player.Reset(Config.PLAYER_ACCELEARION, Config.PLAYER_MAX_VELOCITY, Config.PLAYER_FRICTION);

	for (auto &gem : objects.Gems)
	{
		gem.Randomize(screenWidth, screenHeight);
	}

	for (auto &asteroid : objects.Asteroids)
	{
		asteroid.Randomize(screenWidth, screenHeight);
	}

	objects.Musics.at("backgroundMusic").Stop();
	objects.Musics.at("menuMusic").Play();

	return;
}

//* Game modes
void Game::GameTitleScreen()
{
	SDL_RenderClear(Renderer);

	RenderObjects(currentGameState);

	SDL_RenderPresent(Renderer);

	return;
}

void Game::GameStarted()
{
	HandlePlayers();
	HandleGems();
	HandleAsteroids();
	CheckCollisions();
	AnimateObjects();

	SDL_RenderClear(Renderer);

	RenderObjects(currentGameState);

	SDL_RenderPresent(Renderer);

	return;
}

void Game::GamePaused()
{
	SDL_RenderClear(Renderer);

	RenderObjects(lastGameState);
	RenderObjects(currentGameState);

	SDL_RenderPresent(Renderer);

	return;
}

void Game::GameOver()
{
	SDL_RenderClear(Renderer);

	RenderObjects(currentGameState);

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
	if (Score > Highscore)
	{
		Highscore = Score;
	}

	std::wstring current_score = std::to_wstring(Score);
	Text &scoreText = objects.GetText("scoreText");
	scoreText.SetMessage(current_score);
	scoreText.SetRectPos(static_cast<float>(screenWidth), 0, NE);

	return;
}

void Game::HandlePlayers()
{
	objects.Player.HandleInput(screenWidth, screenHeight, gameClock.deltaTimeSeconds, Keyboard);

	return;
}

void Game::HandleAsteroids()
{
	for (auto &asteroid : objects.Asteroids)
	{
		asteroid.Move(screenWidth, screenHeight, gameClock.deltaTimeSeconds);
	}

	return;
}

void Game::HandleGems()
{
	for (auto &gem : objects.Gems)
	{
		gem.Blink(gameClock.deltaTimeSeconds);
	}

	return;
}

void Game::CheckCollisions()
{
	for (auto &gem : objects.Gems)
	{
		if (!gem.Collideswith(objects.Player))
		{
			continue;
		}

		UpdateScore(1);
		objects.Chunks.at("gemCollected").Play();

		gem.Randomize(screenWidth, screenHeight);
		gem.RestartTimers();

		objects.Player.SetMaxVelocity(objects.Player.GetMaxVelocity() + Config.PLAYER_MAX_VELOCITY_BOOST);
		objects.Player.SetAcceleration(objects.Player.GetAcceleration() + Config.PLAYER_ACCELEARION_BOOST);
		objects.Player.StartEffect();
	}

	for (const auto &asteroid : objects.Asteroids)
	{
		if (!asteroid.Collideswith(objects.Player))
		{
			continue;
		}

		HandleGameOver();
	}

	return;
}

void Game::HandleGameOver()
{
	currentGameState = GAME_OVER;

	for (auto &gem : objects.Gems)
	{
		gem.StopTimers();
	}

	std::wstring current_score = std::to_wstring(Score);
	std::wstring high_score = std::to_wstring(Highscore);

	Text &game_over = objects.GetText("gameOver");
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

void Game::RenderObjects(gameState game_state)
{
	std::vector<Text> &Texts = objects.gameModes[game_state].first;
	std::vector<IMGSprite> &IMGSprites = objects.gameModes[game_state].second;

	for (const auto &element : IMGSprites)
	{
		element.Render();
	}

	if (game_state == GAME_STARTED)
	{
		for (const auto &element : objects.Asteroids)
		{
			element.Render();
		}
		for (const auto &element : objects.Gems)
		{
			element.Render();
		}

		objects.Player.Render();
	}

	for (const auto &element : Texts)
	{
		element.Render();
	}

	return;
}

void Game::AnimateObjects()
{

	objects.Player.Animate(currentTicks);

	for (auto &element : objects.Gems)
	{
		element.Animate(currentTicks);
	}

	return;
}