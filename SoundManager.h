#pragma once
#include "Audio.h"

class SoundManager {
public:
	SoundManager();

	~SoundManager();

	enum class Sound {
		SE_DARUMA_BREAK
	};

	void OnPlaySound(const Sound& soundType);

private:
	Audio* audio_ = nullptr;

	uint32_t seDarumaBreakData_ = 0;
	Sound sound_ = Sound::SE_DARUMA_BREAK;
};
