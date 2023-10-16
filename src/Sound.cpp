#include "Sound.hpp"

#include <SDL2/SDL_mixer.h>

/*------------ soundChunk ------------*/

//*non-static(private)

void soundChunk::Init()
{
	Chunk = Mix_LoadWAV(Path);

	return;
}

//*non-static(public)

soundChunk::soundChunk(const char *path)
	: Path(path)
{
	Init();

	return;
}

soundChunk::~soundChunk()
{
	Mix_FreeChunk(Chunk);
	Chunk = nullptr;

	Path = nullptr;

	return;
}

Mix_Chunk *soundChunk::GetChunk() const
{
	return Chunk;
}

void soundChunk::PlayChunk()
{
	Channel = Mix_PlayChannel(-1, Chunk, 0);

	return;
}

int soundChunk::GetChannel() const
{
	return Channel;
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
		Mix_VolumeChunk(Chunk, MIX_MAX_VOLUME);
		return;
	}

	Volume = new_volume;
	Mix_VolumeChunk(Chunk, new_volume);

	return;
}

/*------------ soundMusic ------------*/

//*non-static(private)

void soundMusic::Init()
{
	Music = Mix_LoadMUS(Path);

	return;
}

//*non-static(public)

soundMusic::soundMusic(const char *path) : Path(path)
{
	Init();

	return;
}

soundMusic::~soundMusic()
{
	Mix_FreeMusic(Music);
	Music = nullptr;

	Path = nullptr;

	return;
}

Mix_Music *soundMusic::GetMusic() const
{
	return Music;
}

void soundMusic::PlayMusic()
{
	Mix_PlayMusic(Music, -1);

	return;
}