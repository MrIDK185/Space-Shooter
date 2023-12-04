#ifndef SOUND_HPP
#define SOUND_HPP

#include <string>
#include <array>

#include <SDL2/SDL_mixer.h>

struct channelControl;

typedef enum
{
	PLAYING,
	PAUSED,
	STOPPED
} soundState;

/*------------ soundChunk ------------*/

class soundChunk
{
private:
	//*non-static

	std::string Path;

	Mix_Chunk *Chunk = nullptr;
	int Channel = -1;
	int Volume;
	soundState currentSoundState = STOPPED;

	channelControl *channelController = nullptr;

	void LoadChunk();

public:
	//*non-static

	explicit soundChunk(std::string path, channelControl *controller);

	~soundChunk();

	std::string GetPath() const;

	void SetPath(std::string path);

	Mix_Chunk *GetChunk() const;

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
	//*non-static

	std::string Path;

	Mix_Music *Music = nullptr;
	soundState currentSoundState = STOPPED;

	void LoadMusic();

public:
	//*non-static

	explicit soundMusic(std::string path);

	~soundMusic();

	Mix_Music *GetMusic() const;

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