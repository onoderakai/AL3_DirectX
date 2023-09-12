#include "SoundManager.h"
#include "ImGuiManager.h"

SoundManager::SoundManager() {
	audio_ = Audio::GetInstance();
	bgmTitleData_ = audio_->LoadWave("Sounds/BGM_title2.wav");
	bgmStageData_ = audio_->LoadWave("Sounds/BGM_title.wav");
	seDarumaBreakData_ = audio_->LoadWave("Sounds/se_daruma_break.wav");
	seMissTouchData_ = audio_->LoadWave("Sounds/se_miss_touch.wav");
	seIchatchCountData_ = audio_->LoadWave("Sounds/se_ichatch_count.wav");
	seIchatchStartData_ = audio_->LoadWave("Sounds/se_ichatch_start.wav");
	seSelectRBLBData_ = audio_->LoadWave("Sounds/se_select_RB_LB.wav");
	seExplainData_ = audio_->LoadWave("Sounds/se_explain.wav");
	seFinishData_ = audio_->LoadWave("Sounds/se_finish.wav");
}

SoundManager::~SoundManager() {}

SoundManager* SoundManager::GetInstance() {
	static SoundManager instance;
	return &instance;
}

void SoundManager::OnPlaySound(const Sound& soundType) {
	switch (soundType) {
	case Sound::SE_DARUMA_BREAK:
		audio_->PlayWave(seDarumaBreakData_);
		break;
	case Sound::SE_MISS_TOUCH:
		audio_->PlayWave(seMissTouchData_);
		break;
	case Sound::SE_ICHATCH_COUNT:
		audio_->PlayWave(seIchatchCountData_);
		break;
	case Sound::SE_ICHATCH_START:
		audio_->PlayWave(seIchatchStartData_);
		break;
	case Sound::SE_SELECT_RB_LB:
		audio_->PlayWave(seSelectRBLBData_);
		break;
	case Sound::SE_EXPLAIN:
		audio_->PlayWave(seExplainData_);
		break;
	case Sound::SE_FINISH:
		audio_->PlayWave(seFinishData_);
		break;
	default:
		break;
	}
}

void SoundManager::OnPlayBGM(const BGM& bgmType) {
	switch (bgmType) {
	case BGM::BGM_TITLE:
		audio_->PlayWave(bgmTitleData_, true, 1.0f);
		break;
	case BGM::BGM_TIME_ATTACK:
		if (audio_->PlayWave(bgmStageData_) == -1) {
		}
		audio_->PlayWave(bgmStageData_);
		break;
	default:
		break;
	}
}

void SoundManager::AllStopSound() {
	audio_->StopWave(seDarumaBreakData_);
	audio_->StopWave(seMissTouchData_);
	audio_->StopWave(bgmTitleData_);
	audio_->StopWave(bgmStageData_);
	audio_->StopWave(seIchatchCountData_);
	audio_->StopWave(seIchatchStartData_);
	audio_->StopWave(seSelectRBLBData_);
	audio_->StopWave(seExplainData_);
	audio_->StopWave(seFinishData_);
}
