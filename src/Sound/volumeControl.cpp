#include <SDL2/SDL_mixer.h>

#include "volumeControl.hpp"

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