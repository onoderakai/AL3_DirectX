#include "SoundManager.h"

SoundManager::SoundManager() {
	audio_ = Audio::GetInstance();
	seDarumaBreakData_ = audio_->LoadWave("Sounds/se_daruma_break.wav");
}

SoundManager::~SoundManager() {}

void SoundManager::OnPlaySound(const Sound& soundType) {
	switch (soundType) {
	case Sound::SE_DARUMA_BREAK:
		audio_->PlayWave(seDarumaBreakData_);
		break;
	default:
		break;
	}
}
