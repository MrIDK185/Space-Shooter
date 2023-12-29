#include "Game/EventHandler.hpp"
#include "Game/EventType.hpp"
#include "Game/EventCode.hpp"
#include "Game/Game.hpp"
#include "Sprite/Text.hpp"
#include "Sprite/Gem.hpp"

#include <SDL2/SDL_mixer.h>

//* non-static(private)

bool EventHandler::Handle_WindowQuit()
{
	currentGame->Running = false;

	return true;
}

bool EventHandler::Handle_Q()
{
	currentGame->Running = false;

	return true;
}

bool EventHandler::Handle_Escape()
{
	if (currentGame->currentGameState != GAME_PAUSED)
	{
		currentGame->pausedTicks = SDL_GetTicks64();
		currentGame->volumeController.Mute();
		currentGame->lastGameState = currentGame->currentGameState;
		currentGame->currentGameState = GAME_PAUSED;
		return true;
	}

	currentGame->totalTimePaused += SDL_GetTicks64() - currentGame->pausedTicks;
	currentGame->volumeController.Unmute();
	currentGame->currentGameState = currentGame->lastGameState;

	return true;
}

bool EventHandler::Handle_Return()
{
	if (currentGame->currentGameState == GAME_STARTED || currentGame->startTimer.Started)
	{
		return false;
	}

	if (currentGame->currentGameState == GAME_OVER)
	{
		currentGame->Reset();
		return true;
	}

	std::wstring countdown_str = std::to_wstring(currentGame->startTimer.counterMilliseconds / 1000);
	Text &startText = currentGame->objectsTitleScreen.Texts.at("startText");

	startText.SetMessage(countdown_str.c_str());
	startText.SetRectPos(currentGame->screenWidth / 2, currentGame->screenHeight / 2);

	currentGame->objectsTitleScreen.Musics.at("menuMusic").Stop();
	currentGame->objectsTitleScreen.Chunks.at("startSound").Play();

	currentGame->startTimer.Start();

	return true;
}

bool EventHandler::Handle_F11()
{
	Uint32 window_flags = SDL_GetWindowFlags(currentGame->Window);
	if ((window_flags & SDL_WINDOW_FULLSCREEN) != SDL_WINDOW_FULLSCREEN)
	{
		SDL_SetWindowFullscreen(currentGame->Window, SDL_WINDOW_FULLSCREEN);
		SDL_ShowCursor(SDL_DISABLE);

		return true;
	}

	SDL_SetWindowFullscreen(currentGame->Window, 0);
	SDL_ShowCursor(SDL_ENABLE);

	return true;
}

bool EventHandler::Handle_KP_Plus()
{
	currentGame->volumeController.changeMasterVolume(currentGame->volumeController.masterVolume + 10);

	return true;
}

bool EventHandler::Handle_KP_Minus()
{
	currentGame->volumeController.changeMasterVolume(currentGame->volumeController.masterVolume - 10);

	return true;
}

bool EventHandler::Handle_M()
{
	currentGame->volumeController.toggleMute();

	return true;
}

bool EventHandler::Handle_TimerEvent()
{
	bool retval = false;

	switch (Event.user.code)
	{
	case COUNTDOWN_DECREMENT:
		printf("EVENT: Countdown Decrement\n");
		retval = Handle_CountdownDecrement();
		break;

	case GAME_START:
		printf("EVENT: Game Start\n");
		retval = Handle_GameStart();
		break;

	case GEM_BLINK:
		printf("EVENT: Gem Blink\n");
		retval = Handle_GemBlink();
		break;

	case GEM_DISAPPEAR:
		printf("EVENT: Gem Disappear\n");
		retval = Handle_GemDisappear();
		break;

	case PLAYER_EFFECT_STOP:
		printf("EVENT: Player Effect Stop\n");
		retval = Handle_PlayerEffectStop();
		break;

	case NO_EVENT:
	default:
		break;
	}

	return retval;
}

bool EventHandler::Handle_CountdownDecrement()
{
	std::wstring countdown_str = std::to_wstring(currentGame->startTimer.counterMilliseconds / 1000);
	Text &startText = currentGame->objectsTitleScreen.Texts.at("startText");

	startText.SetMessage(countdown_str);
	startText.SetRectPos(currentGame->screenWidth / 2, currentGame->screenHeight / 2);

	return true;
}

bool EventHandler::Handle_GameStart()
{
	currentGame->startTimer.Stop();

	currentGame->currentGameState = GAME_STARTED;

	for (auto &gem : currentGame->objectsGameRunning.Gems)
	{
		gem.Randomize(currentGame->screenWidth, currentGame->screenHeight);
	}

	currentGame->objectsGameRunning.Musics.at("backgroundMusic").Play();

	return true;
}

bool EventHandler::Handle_GemBlink()
{
	static_cast<Gem *>(Event.user.data2)->SetBlinking(true);

	return true;
}

bool EventHandler::Handle_GemDisappear()
{
	static_cast<Gem *>(Event.user.data2)->Randomize(currentGame->screenWidth, currentGame->screenHeight);
	currentGame->objectsGameRunning.Chunks.at("gemMissed").Play();
	currentGame->UpdateScore(-1);

	return true;
}

bool EventHandler::Handle_PlayerEffectStop()
{
	static_cast<Player *>(Event.user.data2)->StopEffect();

	return true;
}

void EventHandler::HandleGameStarted()
{
	while (SDL_PollEvent(&Event))
	{
		switch (Event.type)
		{
		case SDL_WINDOWEVENT:
			switch (Event.window.event)
			{
			case SDL_WINDOWEVENT_CLOSE:
				Handle_WindowQuit();
				return;
			default:
				break;
			}
			break;

		case SDL_USEREVENT:
			switch (Event.user.type)
			{
			case USEREVENT_TIMER:
				Handle_TimerEvent();
				break;
			default:
				break;
			}
			break;

		case SDL_KEYDOWN:
			switch (Event.key.keysym.sym)
			{
			case SDLK_q:
				Handle_Q();
				return;
			case SDLK_ESCAPE:
				Handle_Escape();
				break;
			case SDLK_RETURN:
				Handle_Return();
				break;
			case SDLK_F11:
				Handle_F11();
				break;
			case SDLK_KP_PLUS:
				Handle_KP_Plus();
				break;
			case SDLK_KP_MINUS:
				Handle_KP_Minus();
				break;
			case SDLK_m:
				Handle_M();
				break;
			default:
				break;
			}
			break;

		default:
			break;
		}
	}

	return;
}

void EventHandler::HandleGamePaused()
{
	while (SDL_PollEvent(&Event))
	{
		if (Event.window.event == SDL_WINDOWEVENT_CLOSE)
		{
			Handle_WindowQuit();
			return;
		}

		if (Event.type != SDL_KEYDOWN)
		{
			return;
		}

		switch (Event.key.keysym.sym)
		{
		case SDLK_q:
			Handle_Q();
			return;
		case SDLK_ESCAPE:
			Handle_Escape();
			break;
		case SDLK_F11:
			Handle_F11();
			break;
		default:
			break;
		}
	}

	return;
}

//* non-static(public)

EventHandler::EventHandler()
	: currentGame(nullptr)
{
}

EventHandler::EventHandler(Game *current_game)
	: currentGame(current_game)
{
}

EventHandler::~EventHandler()
{
	currentGame = nullptr;

	return;
}

void EventHandler::HandleEvents()
{
	if (currentGame->currentGameState == GAME_PAUSED)
	{
		HandleGamePaused();
		return;
	}

	HandleGameStarted();

	return;
}