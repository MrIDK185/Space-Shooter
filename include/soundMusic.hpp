#ifndef SOUND_MUSIC_HPP
#define SOUND_MUSIC_HPP

#include <string>
#include <memory>

#include <SDL2/SDL_mixer.h>

#include "soundState.hpp"

template <typename T>
using unique_ptr_deleter = std::unique_ptr<T, void (*)(T *)>;

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

	explicit soundMusic(soundMusic &&obj);

	~soundMusic();

	unique_ptr_deleter<Mix_Music> GetMusic();

	void SetMusic(Mix_Music *music);

	soundState GetCurrentSoundState();

	void Play();

	void Pause();

	void Resume();

	void Stop();
};

#endif //! SOUND_MUSIC_HPP