#include "EventHandler.hpp"
#include "Game.hpp"
#include "Text.hpp"

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
	if (currentGame->currentGameState == GAME_STARTED || currentGame->currentGameState == GAME_PAUSED || currentGame->startCountdown)
	{
		return false;
	}

	std::wstring countdown_str = std::to_wstring(currentGame->Countdown);
	currentGame->textMap.at("startText")->SetMessage(countdown_str.c_str());

	SDL_FRect text_rect = currentGame->textMap.at("startText")->GetRect();
	currentGame->textMap.at("startText")->SetRectPos(currentGame->screenWidth / 2 - text_rect.w / 2, currentGame->screenHeight / 2 - text_rect.h / 2);

	currentGame->startCountdown = true;
	currentGame->startTimer.Start(currentGame->currentTicks);

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
	if (currentGame->currentSoundState == MUTED)
	{
		return false;
	}

	currentGame->currentChunkVolume + 10 <= MIX_MAX_VOLUME ? currentGame->currentChunkVolume += 10 : currentGame->currentChunkVolume = MIX_MAX_VOLUME;
	Mix_MasterVolume(currentGame->currentChunkVolume);
	Mix_VolumeMusic(currentGame->currentChunkVolume / 5);

	return true;
}

bool EventHandler::Handle_KP_Minus()
{
	if (currentGame->currentSoundState == MUTED)
	{
		return false;
	}

	currentGame->currentChunkVolume - 10 >= 0 ? currentGame->currentChunkVolume -= 10 : currentGame->currentChunkVolume = 0;
	Mix_MasterVolume(currentGame->currentChunkVolume);
	Mix_VolumeMusic(currentGame->currentChunkVolume / 5);

	return true;
}

bool EventHandler::Handle_M()
{
	if (currentGame->currentSoundState == UNMUTED)
	{
		currentGame->lastChunkVolume = currentGame->currentChunkVolume;

		Mix_MasterVolume(0);
		Mix_VolumeMusic(0);

		currentGame->currentSoundState = MUTED;

		return true;
	}

	Mix_MasterVolume(currentGame->lastChunkVolume);
	Mix_VolumeMusic(currentGame->lastChunkVolume / 5);

	currentGame->currentSoundState = UNMUTED;

	return true;
}

//* non-static(public)

EventHandler::EventHandler(Game *current_game) : currentGame(current_game)
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