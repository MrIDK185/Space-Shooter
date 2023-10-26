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
	currentGame->textMap.at("startText")->SetMessage(countdown_str.c_str());

	SDL_FRect text_rect = currentGame->textMap.at("startText")->GetRect();
	currentGame->textMap.at("startText")->SetRectPos(currentGame->screenWidth / 2 - text_rect.w / 2, currentGame->screenHeight / 2 - text_rect.h / 2);

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

bool EventHandler::Handle_TimerDecrement()
{
	std::wstring countdown_str = std::to_wstring(currentGame->startTimer.counterMilliseconds / 1000);
	currentGame->textMap.at("startText")->SetMessage(countdown_str);

	currentGame->textMap.at("scoreText")->SetRectPos(static_cast<float>(currentGame->screenWidth) - currentGame->textMap.at("scoreText")->GetRect().w, 0);

	return true;
}

bool EventHandler::Handle_TimerStop()
{
	currentGame->startTimer.Stop();

	currentGame->currentGameState = GAME_STARTED;

	currentGame->IMGSpriteMap.erase("startBackground");
	currentGame->textMap.erase("startText");

	for (std::shared_ptr<Gem> gem : currentGame->gemGroup)
	{
		gem->UpdateTicks(currentGame->currentTicks);
	}

	Mix_HaltMusic();
	currentGame->chunkMap.at("startSound")->PlayChunk();
	currentGame->startSoundPlaying = true;

	return true;
}

//* non-static(public)

EventHandler::EventHandler(Game *current_game)
	: currentGame(current_game)
{
	SDL_RegisterEvents(2);
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