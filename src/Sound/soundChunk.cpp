#include "Sound/soundChunk.hpp"
#include "Sound/channelControl.hpp"

//* non-static(private)

void soundChunk::LoadChunk()
{
	SetChunk(Mix_LoadWAV(Path.c_str()));

	return;
}

//* non-static(public)

soundChunk::soundChunk(std::string path, channelControl *controller)
	: Path(path),
	  Volume(MIX_MAX_VOLUME),
	  channelController(controller)
{
	LoadChunk();

	return;
}

soundChunk::soundChunk(const soundChunk &obj)
	: Path(obj.Path),
	  Volume(obj.Volume),
	  channelController(obj.channelController)
{
	LoadChunk();

	return;
}

soundChunk::soundChunk(soundChunk &&obj)
	: Path(obj.Path),
	  Chunk(std::move(obj.Chunk)),
	  Channel(obj.Channel),
	  Volume(obj.Volume),
	  currentSoundState(obj.currentSoundState),
	  channelController(obj.channelController)
{
}

soundChunk::~soundChunk()
{
	Chunk = nullptr;
	channelController = nullptr;

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

unique_ptr_deleter<Mix_Chunk> soundChunk::GetChunk()
{
	return std::move(Chunk);
}

void soundChunk::SetChunk(Mix_Chunk *chunk)
{
	Chunk.reset(chunk);

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
	Mix_VolumeChunk(Chunk.get(), Volume);

	return;
}

soundState soundChunk::GetCurrentSoundState()
{
	return currentSoundState;
}

void soundChunk::SetCurrentSoundState(soundState new_state)
{
	if (currentSoundState == new_state)
	{
		return;
	}

	currentSoundState = new_state;

	return;
}

void soundChunk::Play()
{
	Channel = channelController->requestFreeChannel(this);

	if (Channel < 0)
	{
		return;
	}

	if (Mix_PlayChannel(Channel, Chunk.get(), 0) >= 0)
	{
		currentSoundState = PLAYING;
	}

	return;
}

void soundChunk::Pause()
{
	if (Channel < 0 || currentSoundState == PAUSED || currentSoundState == STOPPED)
	{
		return;
	}

	Mix_Pause(Channel);
	currentSoundState = PAUSED;

	return;
}

void soundChunk::Resume()
{
	if (Channel < 0 || currentSoundState == PLAYING || currentSoundState == STOPPED)
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