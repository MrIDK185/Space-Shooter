#ifndef VOLUME_CONTROL_HPP
#define VOLUME_CONTROL_HPP

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

#endif //! VOLUME_CONTROL_HPP