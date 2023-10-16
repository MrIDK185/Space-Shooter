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

void soundChunk::SetChannel(unsigned int channel)
{
	Channel = channel;

	return;
}

unsigned int soundChunk::GetVolume() const
{
	return Volume;
}

void soundChunk::SetVolume(unsigned int new_volume)
{
	if (new_volume > MIX_MAX_VOLUME)
	{
		Volume = MIX_MAX_VOLUME;
		Mix_VolumeChunk(Chunk, Volume);

		return;
	}

	Volume = new_volume;
	Mix_VolumeChunk(Chunk, new_volume);

	return;
}

void soundChunk::PlayChunk()
{
	Channel = Mix_PlayChannel(-1, Chunk, 0);

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

void soundMusic::PlayMusic()
{
	Mix_PlayMusic(Music, -1);

	return;
}