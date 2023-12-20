#ifndef SOUND_HPP
#define SOUND_HPP

#include <string>
#include <array>
#include <memory>

#include <SDL2/SDL_mixer.h>

template <typename T>
using unique_ptr_deleter = std::unique_ptr<T, void (*)(T *)>;

typedef enum
{
	PLAYING,
	PAUSED,
	STOPPED
} soundState;

struct channelControl;

/*------------ soundChunk ------------*/

class soundChunk
{
private:
	//* static

	static inline void chunkDestructor(Mix_Chunk *chunk) { Mix_FreeChunk(chunk); }

	//* non-static

	std::string Path;

	unique_ptr_deleter<Mix_Chunk> Chunk = {nullptr, chunkDestructor};
	int Channel = -1;
	int Volume;
	soundState currentSoundState = STOPPED;

	channelControl *channelController = nullptr;

	void LoadChunk();

public:
	//* non-static

	explicit soundChunk(std::string path, channelControl *controller);

	~soundChunk();

	std::string GetPath() const;

	void SetPath(std::string path);

	unique_ptr_deleter<Mix_Chunk> GetChunk();

	void SetChunk(Mix_Chunk *chunk);

	int GetChannel() const;

	void SetChannel(int channel);

	int GetVolume() const;

	void SetVolume(int new_volume);

	soundState GetCurrentSoundState();

	void SetCurrentSoundState(soundState new_state);

	void Play();

	void Pause();

	void Resume();

	void Stop();
};

/*------------ soundMusic ------------*/

class soundMusic
{
private:
	//* static

	static inline void musicDestructor(Mix_Music *music) { Mix_FreeMusic(music); }

	//* non-static

	std::string Path;

	unique_ptr_deleter<Mix_Music> Music = {nullptr, musicDestructor};
	soundState currentSoundState = STOPPED;

	void LoadMusic();

public:
	//* non-static

	explicit soundMusic(std::string path);

	~soundMusic();

	unique_ptr_deleter<Mix_Music> GetMusic();

	void SetMusic(Mix_Music *music);

	soundState GetCurrentSoundState();

	void Play();

	void Pause();

	void Resume();

	void Stop();
};

/*------------ volumeControl ------------*/

typedef enum
{
	UNMUTED,
	MUTED
} volumeState;

struct volumeControl
{
	int
		masterVolume = 0,
		lastMasterVolume = 0,
		musicVolume = 0,
		lastMusicVolume = 0;

	float ratioMusicToChunk = 0.2;

	volumeState currentVolumeState = UNMUTED;

	void changeMasterVolume(int volume);

	void saveMasterVolume();

	void Mute();

	void Unmute();

	void toggleMute();

	volumeControl() = default;
	~volumeControl() = default;
};

/*------------ ChannelControl ------------*/

struct channelControl
{
private:
	std::array<bool, MIX_CHANNELS> freeChannels = {true};
	std::array<soundChunk *, MIX_CHANNELS> usedChannels = {nullptr};

public:
	int getFreeChannel(soundChunk *chunk);
	void channelFinished(int channel);

	channelControl() = default;
	~channelControl() = default;
};

#endif //! SOUND_HPP