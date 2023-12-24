#include "channelControl.hpp"
#include "soundChunk.hpp"

int channelControl::getFreeChannel(soundChunk *chunk)
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