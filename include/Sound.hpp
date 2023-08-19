#ifndef SOUND_HPP
#define SOUND_HPP

constexpr unsigned int MIX_MAX_VOLUME = 128;

typedef struct Mix_Chunk Mix_Chunk;
typedef struct _Mix_Music Mix_Music;

/*------------ soundChunk ------------*/

class soundChunk
{
private:
	//*non-static

	Mix_Chunk *Chunk = nullptr;
	const char *Path = nullptr;
	int Channel;
	unsigned int Volume = MIX_MAX_VOLUME;

	void Init();

public:
	//*non-static

	explicit soundChunk(const char *path);

	~soundChunk();

	Mix_Chunk *GetChunk();

	void PlayChunk();

	int GetChannel();

	unsigned int GetVolume();

	void SetVolume(unsigned int new_volume);
};

/*------------ soundMusic ------------*/

class soundMusic
{
private:
	//*non-static

	Mix_Music *Music = nullptr;
	const char *Path = nullptr;

	void Init();

public:
	//*non-static

	explicit soundMusic(const char *path);

	~soundMusic();

	Mix_Music *GetMusic();

	void PlayMusic();
};

#endif //! SOUND_HPP