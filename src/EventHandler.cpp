#include "EventHandler.hpp"
#include "Game.hpp"
#include "Text.hpp"
#include "Gem.hpp"

#include <SDL2/SDL_mixer.h>

//* non-static(private)

bool EventHandler::Handle_WindowQuit()
{
	if (Event.type != SDL_WINDOWEVENT || Event.window.event != SDL_WINDOWEVENT_CLOSE)
	{
		return false;
	}

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
	if (currentGame->currentGameState == GAME_STARTED || currentGame->currentGameState == GAME_PAUSED || currentGame->startTimer.Started)
	{
		return false;
	}

	if (currentGame->currentGameState == GAME_OVER)
	{
		currentGame->Reset();
		return true;
	}

	std::wstring countdown_str = std::to_wstring(currentGame->startTimer.counterMilliseconds / 1000);
	std::unique_ptr<Text> &startText = currentGame->objectsTitleScreen.Texts.at("startText");

	startText->SetMessage(countdown_str.c_str());
	startText->SetRectPos(currentGame->screenWidth / 2, currentGame->screenHeight / 2);

	currentGame->objectsTitleScreen.Musics.at("menuMusic")->Stop();
	currentGame->objectsTitleScreen.Chunks.at("startSound")->Play();

	currentGame->startTimer.Start();

	return true;
}

bool EventHandler::Handle_F11()
{
	if (currentGame->currentGameState == GAME_PAUSED)
	{
		return false;
	}

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
	if (currentGame->currentGameState == GAME_PAUSED)
	{
		return false;
	}

	currentGame->volumeController.changeMasterVolume(currentGame->volumeController.masterVolume + 10);

	return true;
}

bool EventHandler::Handle_KP_Minus()
{
	if (currentGame->currentGameState == GAME_PAUSED)
	{
		return false;
	}

	currentGame->volumeController.changeMasterVolume(currentGame->volumeController.masterVolume - 10);

	return true;
}

bool EventHandler::Handle_M()
{
	if (currentGame->currentGameState == GAME_PAUSED)
	{
		return false;
	}

	currentGame->volumeController.toggleMute();

	return true;
}

bool EventHandler::Handle_TimerDecrement()
{
	if (currentGame->currentGameState == GAME_PAUSED)
	{
		return false;
	}

	std::wstring countdown_str = std::to_wstring(currentGame->startTimer.counterMilliseconds / 1000);
	std::unique_ptr<Text> &startText = currentGame->objectsTitleScreen.Texts.at("startText");

	startText->SetMessage(countdown_str);
	startText->SetRectPos(currentGame->screenWidth / 2, currentGame->screenHeight / 2);

	return true;
}

bool EventHandler::Handle_TimerStop()
{
	if (currentGame->currentGameState == GAME_PAUSED)
	{
		return false;
	}

	currentGame->startTimer.Stop();

	currentGame->currentGameState = GAME_STARTED;

	for (const auto &gem : currentGame->objectsGameRunning.Gems)
	{
		gem->UpdateTicks(currentGame->currentTicks);
	}

	currentGame->objectsTitleScreen.Chunks.at("startSound")->Stop();
	currentGame->objectsGameRunning.Musics.at("backgroundMusic")->Play();

	return true;
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
}

void EventHandler::HandleEvents()
{
	while (SDL_PollEvent(&Event))
	{
		if (Handle_WindowQuit())
		{
			return;
		}

		if (Event.type == SDL_USEREVENT)
		{
			switch (Event.user.code)
			{
			case TIMER_DECREMENT:
				Handle_TimerDecrement();
				break;
			case TIMER_STOP:
				Handle_TimerStop();
				break;
			default:
				break;
			}
		}

		if (Event.type != SDL_KEYDOWN)
		{
			continue;
		}

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
	}

	return;
}