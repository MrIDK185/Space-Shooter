#ifndef CHANNEL_CONTROL_HPP
#define CHANNEL_CONTROL_HPP

#include <array>

#include <SDL2/SDL_mixer.h>

class soundChunk;
class soundMusic;

struct channelControl
{
private:
	unsigned int activeChannels = 0;
	std::array<soundChunk *, MIX_CHANNELS> usedChannels = {nullptr};

	bool musicPlaying = false;
	soundMusic *Music = nullptr;

public:
	int requestFreeChannel(soundChunk *chunk);
	void channelFinished(int channel);
	bool requestFreeMusic(soundMusic *music);
	void musicFinished();

	channelControl() = default;
	~channelControl() = default;
};

#endif //! CHANNEL_CONTROL_HPP