#ifndef CHANNEL_CONTROL_HPP
#define CHANNEL_CONTROL_HPP

#include <array>

#include <SDL2/SDL_mixer.h>

class soundChunk;

struct channelControl
{
private:
	unsigned int activeChannels = 0;
	std::array<soundChunk *, MIX_CHANNELS> usedChannels = {nullptr};

public:
	int getFreeChannel(soundChunk *chunk);
	void channelFinished(int channel);

	channelControl() = default;
	~channelControl() = default;
};

#endif //! CHANNEL_CONTROL_HPP