#include "soundMusic.hpp"

//* non-static(private)

void soundMusic::LoadMusic()
{
	SetMusic(Mix_LoadMUS(Path.c_str()));

	return;
}

//* non-static(public)

soundMusic::soundMusic(std::string path)
	: Path(path)
{
	LoadMusic();

	return;
}

soundMusic::~soundMusic()
{
	Music = nullptr;

	return;
}

unique_ptr_deleter<Mix_Music> soundMusic::GetMusic()
{
	return std::move(Music);
}

void soundMusic::SetMusic(Mix_Music *music)
{
	Music.reset(music);

	return;
}

soundState soundMusic::GetCurrentSoundState()
{
	return currentSoundState;
}

void soundMusic::Play()
{
	if (Mix_PlayMusic(Music.get(), -1) == 0)
	{
		currentSoundState = PLAYING;
	}

	return;
}

void soundMusic::Pause()
{
	if (currentSoundState == PAUSED | currentSoundState == STOPPED)
	{
		return;
	}

	Mix_PauseMusic();
	currentSoundState = PAUSED;

	return;
}

void soundMusic::Resume()
{
	if (currentSoundState == PLAYING | currentSoundState == STOPPED)
	{
		return;
	}

	Mix_ResumeMusic();
	currentSoundState = PLAYING;

	return;
}

void soundMusic::Stop()
{
	if (currentSoundState == STOPPED)
	{
		return;
	}

	Mix_HaltMusic();
	currentSoundState = STOPPED;

	return;
}