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
	currentGame->currentGameState = (currentGame->currentGameState == GAME_PAUSED ? GAME_STARTED : GAME_PAUSED);

	return true;
}

bool EventHandler::Handle_Return()
{
	if (currentGame->currentGameState == GAME_STARTED || currentGame->currentGameState == GAME_PAUSED || currentGame->startTimer.Started)
	{
		return false;
	}

	std::wstring countdown_str = std::to_wstring(currentGame->startTimer.counterMilliseconds / 1000);
	currentGame->objectsTitleScreen.Texts.at("startText")->SetMessage(countdown_str.c_str());

	SDL_FRect text_rect = currentGame->objectsTitleScreen.Texts.at("startText")->GetRect();
	currentGame->objectsTitleScreen.Texts.at("startText")->SetRectPos(currentGame->screenWidth / 2 - text_rect.w / 2, currentGame->screenHeight / 2 - text_rect.h / 2);

	currentGame->startTimer.Start();

	currentGame->objectsTitleScreen.Musics.at("menuMusic")->Stop();
	currentGame->objectsTitleScreen.Chunks.at("startSound")->Play();

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

bool EventHandler::Handle_TimerDecrement()
{
	std::wstring countdown_str = std::to_wstring(currentGame->startTimer.counterMilliseconds / 1000);
	currentGame->objectsTitleScreen.Texts.at("startText")->SetMessage(countdown_str);

	SDL_FRect text_rect = currentGame->objectsTitleScreen.Texts.at("startText")->GetRect();
	currentGame->objectsTitleScreen.Texts.at("startText")->SetRectPos(currentGame->screenWidth / 2 - text_rect.w / 2, currentGame->screenHeight / 2 - text_rect.h / 2);

	return true;
}

bool EventHandler::Handle_TimerStop()
{
	currentGame->startTimer.Stop();

	currentGame->currentGameState = GAME_STARTED;

	for (std::shared_ptr<Gem> gem : currentGame->objectsGameRunning.Gems)
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