#include "soundMusic.hpp"
#include "channelControl.hpp"

//* non-static(private)

void soundMusic::LoadMusic()
{
	SetMusic(Mix_LoadMUS(Path.c_str()));

	return;
}

//* non-static(public)

soundMusic::soundMusic(std::string path, channelControl *channel_control)
	: Path(path),
	  channelController(channel_control)
{
	LoadMusic();

	return;
}

soundMusic::soundMusic(const soundMusic &obj)
	: Path(obj.Path),
	  Music(nullptr, musicDestructor),
	  currentSoundState(obj.currentSoundState),
	  channelController(obj.channelController)
{
	LoadMusic();

	return;
}

soundMusic::soundMusic(soundMusic &&obj)
	: Path(obj.Path),
	  Music(std::move(obj.Music)),
	  currentSoundState(obj.currentSoundState),
	  channelController(obj.channelController)
{
}

soundMusic::~soundMusic()
{
	Music = nullptr;

	return;
}

std::string soundMusic::GetPath()
{
	return Path;
}

void soundMusic::SetPath(std::string path)
{
	Path = path;

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

void soundMusic::SetCurrentSoundState(soundState state)
{
	currentSoundState = state;

	return;
}

void soundMusic::Play()
{
	bool can_play = channelController->requestFreeMusic(this);

	if (can_play)
	{
		Mix_PlayMusic(Music.get(), -1);
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