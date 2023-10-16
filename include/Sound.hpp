#ifndef SOUND_HPP
#define SOUND_HPP

#include <string>

typedef struct Mix_Chunk Mix_Chunk;
typedef struct _Mix_Music Mix_Music;

/*------------ soundChunk ------------*/

class soundChunk
{
private:
	//*non-static

	std::string Path;

	Mix_Chunk *Chunk = nullptr;
	int Channel = -1;
	int Volume;

	void LoadChunk();

public:
	//*non-static

	explicit soundChunk(std::string path);

	~soundChunk();

	std::string GetPath() const;

	void SetPath(std::string path);

	Mix_Chunk *GetChunk() const;

	void SetChunk(Mix_Chunk *chunk);

	int GetChannel() const;

	void SetChannel(int channel);

	int GetVolume() const;

	void SetVolume(int new_volume);

	void PlayChunk();
};

/*------------ soundMusic ------------*/

class soundMusic
{
private:
	//*non-static

	std::string Path;

	Mix_Music *Music = nullptr;

	void LoadMusic();

public:
	//*non-static

	explicit soundMusic(std::string path);

	~soundMusic();

	Mix_Music *GetMusic() const;

	void SetMusic(Mix_Music *music);

	void PlayMusic();
};

/*------------ volumeControl ------------*/

typedef enum
{
	UNMUTED,
	MUTED
} soundState;

struct volumeControl
{
	int
		masterVolume,
		lastMasterVolume,
		musicVolume,
		lastMusicVolume;

	float ratioMusicToChunk = 0.2;

	soundState currentSoundState = UNMUTED;

	void changeMasterVolume(int volume);

	void saveMasterVolume();

	void toggleMute();
};

#endif //! SOUND_HPP