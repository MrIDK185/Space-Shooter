#ifndef SOUND_MUSIC_HPP
#define SOUND_MUSIC_HPP

#include <string>
#include <memory>

#include <SDL2/SDL_mixer.h>

#include "soundState.hpp"

template <typename T>
using unique_ptr_deleter = std::unique_ptr<T, void (*)(T *)>;

struct channelControl;

class soundMusic
{
private:
	//* static

	static inline void musicDestructor(Mix_Music *music) { Mix_FreeMusic(music); }

	//* non-static

	std::string Path;

	unique_ptr_deleter<Mix_Music> Music = {nullptr, musicDestructor};
	soundState currentSoundState = STOPPED;

	channelControl *channelController = nullptr;

	void LoadMusic();

public:
	//* non-static

	explicit soundMusic(std::string path, channelControl *channel_control);

	explicit soundMusic(const soundMusic &obj);

	explicit soundMusic(soundMusic &&obj);

	~soundMusic();

	std::string GetPath();

	void SetPath(std::string path);

	unique_ptr_deleter<Mix_Music> GetMusic();

	void SetMusic(Mix_Music *music);

	soundState GetCurrentSoundState();

	void SetCurrentSoundState(soundState state);

	void Play();

	void Pause();

	void Resume();

	void Stop();
};

#endif //! SOUND_MUSIC_HPP