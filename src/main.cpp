#define SDL_MAIN_HANDLED

#include "Game.hpp"

#include <iostream>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

//* Error-handling
void ShowSDLErrorMessage();

//* Initialization
int InitializeSubsystems();

//* Cleanup
void QuitSubsystems();

//* Error-handling
void ShowSDLErrorMessage()
{
	const char *error_msg = SDL_GetError();
	int displayed = SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Initialization error", error_msg, nullptr);
	if (displayed < 0)
	{
		std::cerr << "Initialization error: " << error_msg << "\n";
		std::cerr << "MessageBox could not be displayed: " << SDL_GetError() << "\n";
	}
	error_msg = nullptr;

	return;
}

//* Initialization
int InitializeSubsystems()
{
	std::cout << "------------- Startup -------------\n\n";

	std::cout << "Initializing SDL...\n";
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0)
	{
		ShowSDLErrorMessage();
		return -1;
	}
	std::cout << "Initializing IMG...\n";
	if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) != (IMG_INIT_JPG | IMG_INIT_PNG))
	{
		ShowSDLErrorMessage();
		return -1;
	}
	std::cout << "Initializing TTF...\n";
	if (TTF_Init() < 0)
	{
		ShowSDLErrorMessage();
		return -1;
	}
	std::cout << "Initializing Mixer...\n";
	if (Mix_Init(MIX_INIT_FLAC | MIX_INIT_MP3) != (MIX_INIT_FLAC | MIX_INIT_MP3))
	{
		ShowSDLErrorMessage();
		return -1;
	}
	std::cout << "Opening audio device...\n\n";
	if (Mix_OpenAudio(44100, AUDIO_F32SYS, 2, 2048) < 0)
	{
		ShowSDLErrorMessage();
		return -1;
	}

	return 0;
}

//* Cleanup
void QuitSubsystems()
{
	std::cout << "\n------------- Cleanup -------------\n\n";

	std::cout << "Closing audio device...\n";
	Mix_CloseAudio();
	std::cout << "Quitting Mixer...\n";
	Mix_Quit();
	std::cout << "Quitting TTF...\n";
	TTF_Quit();
	std::cout << "Quitting IMG...\n";
	IMG_Quit();
	std::cout << "Quitting SDL...\n\n";
	SDL_Quit();
	std::cout << "-------- Cleanup successful --------\n";

	return;
}

int main()
{
	if (InitializeSubsystems() < 0)
	{
		QuitSubsystems();
		return 1;
	}

	Game game;
	int retval = game.Run();
	QuitSubsystems();

	return retval;
}