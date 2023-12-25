#include "channelControl.hpp"
#include "soundChunk.hpp"
#include "soundMusic.hpp"

int channelControl::requestFreeChannel(soundChunk *chunk)
{
	if (activeChannels >= MIX_CHANNELS)
	{
		return -1;
	}

	usedChannels[activeChannels] = chunk;
	++activeChannels;
	return activeChannels - 1;
}

void channelControl::channelFinished(int channel)
{
	usedChannels[channel]->SetCurrentSoundState(STOPPED);
	if (activeChannels > 0)
	{
		--activeChannels;
	}

	return;
}

bool channelControl::requestFreeMusic(soundMusic *music)
{
	if (!musicPlaying)
	{
		Music = music;
		musicPlaying = true;
		return true;
	}

	return false;
}

void channelControl::musicFinished()
{
	Music->SetCurrentSoundState(STOPPED);
	musicPlaying = false;

	return;
}