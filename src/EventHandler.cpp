#include "Configuration.hpp"
#include "Text.hpp"
#include "Time.hpp"
#include "EventHandler.hpp"

#include <SDL2/SDL_mixer.h>

//* non-static(private)

bool EventHandler::Handle_Quit()
{
	if (Event.type != SDL_WINDOWEVENT || Event.window.event != SDL_WINDOWEVENT_CLOSE)
	{
		return false;
	}

	Config.Running = false;

	return true;
}

bool EventHandler::Handle_Q()
{
	if (Event.key.keysym.sym != SDLK_q)
	{
		return false;
	}

	Config.Running = false;

	return true;
}

bool EventHandler::Handle_Escape()
{
	if (Event.key.keysym.sym != SDLK_ESCAPE)
	{
		return false;
	}

	Config.gamePaused ? Config.gamePaused = false : Config.gamePaused = true;

	return true;
}

bool EventHandler::Handle_Return()
{
	if (Event.key.keysym.sym != SDLK_RETURN || Config.gamePaused || Config.gameStarted || Config.startCountdown)
	{
		return false;
	}

	std::wstring countdown_str = std::to_wstring(Config.Countdown);
	Config.textMap.at("startText")->SetMessage(countdown_str.c_str());

	SDL_FRect text_rect = Config.textMap.at("startText")->GetRect();
	Config.textMap.at("startText")->SetRectPos(Config.SCREEN_RESOLUTION_WIDTH / 2 - text_rect.w / 2, Config.SCREEN_RESOLUTION_HEIGHT / 2 - text_rect.h / 2);

	Config.startCountdown = true;
	Config.startTimer.Start(Config.currentTicks);

	return true;
}

bool EventHandler::Handle_F11()
{
	if (Event.key.keysym.sym != SDLK_F11)
	{
		return false;
	}

	Config.windowFlags = SDL_GetWindowFlags(Config.Window);
	if ((Config.windowFlags & SDL_WINDOW_FULLSCREEN) == SDL_WINDOW_FULLSCREEN)
	{
		SDL_SetWindowFullscreen(Config.Window, 0);
		SDL_ShowCursor(SDL_ENABLE);
	}
	else
	{
		SDL_SetWindowFullscreen(Config.Window, SDL_WINDOW_FULLSCREEN);
		SDL_ShowCursor(SDL_DISABLE);
	}

	return true;
}

bool EventHandler::Handle_KP_Plus()
{
	if (Event.key.keysym.sym != SDLK_KP_PLUS || Config.Muted)
	{
		return false;
	}

	Config.currentChunkVolume + 10 <= MIX_MAX_VOLUME ? Config.currentChunkVolume += 10 : Config.currentChunkVolume = MIX_MAX_VOLUME;
	Mix_MasterVolume(Config.currentChunkVolume);
	Mix_VolumeMusic(Config.currentChunkVolume / 5);

	return true;
}

bool EventHandler::Handle_KP_Minus()
{
	if (Event.key.keysym.sym != SDLK_KP_MINUS || Config.Muted)
	{
		return false;
	}

	Config.currentChunkVolume - 10 >= 0 ? Config.currentChunkVolume -= 10 : Config.currentChunkVolume = 0;
	Mix_MasterVolume(Config.currentChunkVolume);
	Mix_VolumeMusic(Config.currentChunkVolume / 5);

	return true;
}

bool EventHandler::Handle_M()
{
	if (Event.key.keysym.sym != SDLK_m)
	{
		return false;
	}

	if (Config.Muted)
	{
		Mix_MasterVolume(Config.lastChunkVolume);
		Mix_VolumeMusic(Config.lastChunkVolume / 5);

		Config.Muted = false;
	}
	else
	{
		Config.lastChunkVolume = Config.currentChunkVolume;

		Mix_MasterVolume(0);
		Mix_VolumeMusic(0);

		Config.Muted = true;
	}

	return true;
}

//* non-static(public)

EventHandler::EventHandler(Configuration &config) : Config(config)
{
}

EventHandler::~EventHandler()
{
}

void EventHandler::HandleEvents()
{
	while (SDL_PollEvent(&Event))
	{
		if (this->Handle_Quit())
		{
			return;
		}

		if (Event.type != SDL_KEYDOWN)
		{
			return;
		}

		if (this->Handle_Q())
		{
			return;
		}
		this->Handle_Escape();
		this->Handle_Return();
		this->Handle_F11();
		this->Handle_KP_Plus();
		this->Handle_KP_Minus();
		this->Handle_M();
	}

	return;
}