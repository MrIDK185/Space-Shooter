#include "Sound.hpp"

#include <SDL2/SDL_mixer.h>

/*------------ soundChunk ------------*/

//*non-static(private)

void soundChunk::LoadChunk()
{
	Chunk = Mix_LoadWAV(Path.c_str());

	return;
}

//*non-static(public)

soundChunk::soundChunk(std::string path)
	: Path(path),
	  Volume(MIX_MAX_VOLUME)
{
	LoadChunk();

	return;
}

soundChunk::~soundChunk()
{
	Mix_FreeChunk(Chunk);
	Chunk = nullptr;

	return;
}

std::string soundChunk::GetPath() const
{
	return Path;
}

void soundChunk::SetPath(std::string path)
{
	Path = path;
	LoadChunk();

	return;
}

Mix_Chunk *soundChunk::GetChunk() const
{
	return Chunk;
}

void soundChunk::SetChunk(Mix_Chunk *chunk)
{
	Chunk = chunk;

	return;
}

int soundChunk::GetChannel() const
{
	return Channel;
}

void soundChunk::SetChannel(int channel)
{
	Channel = channel;

	return;
}

int soundChunk::GetVolume() const
{
	return Volume;
}

void soundChunk::SetVolume(int new_volume)
{
	if (new_volume > MIX_MAX_VOLUME)
	{
		new_volume = MIX_MAX_VOLUME;
	}
	else if (new_volume < 0)
	{
		new_volume = 0;
	}

	Volume = new_volume;
	Mix_VolumeChunk(Chunk, Volume);

	return;
}

soundState soundChunk::GetCurrentSoundState()
{
	return currentSoundState;
}

void soundChunk::Play()
{
	Channel = Mix_PlayChannel(-1, Chunk, 0);
	if (Channel >= 0)
	{
		currentSoundState = PLAYING;
	}

	return;
}

void soundChunk::Pause()
{
	if (Channel < 0 || currentSoundState == PAUSED | currentSoundState == STOPPED)
	{
		return;
	}

	Mix_Pause(Channel);
	currentSoundState = PAUSED;

	return;
}

void soundChunk::Resume()
{
	if (Channel < 0 || currentSoundState == PLAYING | currentSoundState == STOPPED)
	{
		return;
	}

	Mix_Resume(Channel);
	currentSoundState = PLAYING;

	return;
}

void soundChunk::Stop()
{
	if (Channel < 0 || currentSoundState == STOPPED)
	{
		return;
	}

	if (Mix_HaltChannel(Channel) == 0)
	{
		currentSoundState = STOPPED;
	}

	return;
}

/*------------ soundMusic ------------*/

//*non-static(private)

void soundMusic::LoadMusic()
{
	Music = Mix_LoadMUS(Path.c_str());

	return;
}

//*non-static(public)

soundMusic::soundMusic(std::string path)
	: Path(path)
{
	LoadMusic();

	return;
}

soundMusic::~soundMusic()
{
	Mix_FreeMusic(Music);
	Music = nullptr;

	return;
}

Mix_Music *soundMusic::GetMusic() const
{
	return Music;
}

void soundMusic::SetMusic(Mix_Music *music)
{
	Music = music;

	return;
}

soundState soundMusic::GetCurrentSoundState()
{
	return currentSoundState;
}

void soundMusic::Play()
{
	if (Mix_PlayMusic(Music, -1) == 0)
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

/*------------ volumeControl ------------*/

void volumeControl::changeMasterVolume(int volume)
{
	if (currentVolumeState == MUTED)
	{
		return;
	}

	if (volume > MIX_MAX_VOLUME)
	{
		volume = MIX_MAX_VOLUME;
	}
	else if (volume < 0)
	{
		volume = 0;
	}

	masterVolume = volume;
	Mix_MasterVolume(masterVolume);
	musicVolume = volume * ratioMusicToChunk;
	Mix_VolumeMusic(musicVolume);

	return;
}

void volumeControl::saveMasterVolume()
{
	lastMasterVolume = masterVolume;
	lastMusicVolume = musicVolume;

	return;
}

void volumeControl::Mute()
{
	if (currentVolumeState == MUTED)
	{
		return;
	}

	saveMasterVolume();
	changeMasterVolume(0);
	currentVolumeState = MUTED;

	return;
}

void volumeControl::Unmute()
{
	if (currentVolumeState == UNMUTED)
	{
		return;
	}

	currentVolumeState = UNMUTED;
	changeMasterVolume(lastMasterVolume);

	return;
}

void volumeControl::toggleMute()
{
	if (currentVolumeState == UNMUTED)
	{
		Mute();
		return;
	}

	Unmute();

	return;
}