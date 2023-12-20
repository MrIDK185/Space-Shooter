#include "channelControl.hpp"
#include "soundChunk.hpp"

int channelControl::getFreeChannel(soundChunk *chunk)
{
	for (int idx = 0; idx < freeChannels.size(); ++idx)
	{
		if (freeChannels[idx])
		{
			freeChannels[idx] = false;
			usedChannels[idx] = chunk;
			return idx;
		}
	}

	return -1;
}

void channelControl::channelFinished(int channel)
{
	freeChannels[channel] = true;
	usedChannels[channel]->SetCurrentSoundState(STOPPED);

	return;
}
