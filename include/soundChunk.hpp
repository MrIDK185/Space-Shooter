#ifndef SOUND_CHUNK_HPP
#define SOUND_CHUNK_HPP

#include <string>
#include <memory>

#include <SDL2/SDL_mixer.h>

#include "soundState.hpp"

template <typename T>
using unique_ptr_deleter = std::unique_ptr<T, void (*)(T *)>;

struct channelControl;

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

	explicit soundChunk(soundChunk &&obj);

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

#endif //! SOUND_CHUNK_HPP