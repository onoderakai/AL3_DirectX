#include "SceneChange.h"
#include "TextureManager.h"
#include "GameScene.h"

SceneChange* SceneChange::GetInstance() {
	static SceneChange instance;
	return &instance;
}

void SceneChange::Change(SceneNum nextScene, SceneNum* pScene) {
	if (!isLoading_) {
		isLoading_ = true;
		pScene_ = pScene;
		nextScene_ = nextScene;
		addAlpha_ = 1.0f / (float(transitionFrame_) / 2.0f);
	}
}

void SceneChange::Update() {
	// フラグがfalseのときは早期return
	if (!isLoading_) {
		return;
	}

	frameCount_++;
	color.w += addAlpha_;
	loadingSprite_->SetColor(color);
	if (color.w >= 1.0f) {
		addAlpha_ *= -1.0f;
		*pScene_ = nextScene_;

		sound_->AllStopSound();
		switch (nextScene_) {
		case SceneNum::TITLE:
			sound_->OnPlayBGM(SoundManager::BGM::BGM_TITLE);
			break;
		case SceneNum::STAGE_SELECT:
			sound_->OnPlayBGM(SoundManager::BGM::BGM_TIME_ATTACK);
			break;
		case SceneNum::SCORE_ATTACK_STAGE:
			sound_->OnPlayBGM(SoundManager::BGM::BGM_TIME_ATTACK);
			break;
		default:
			break;
		}
		gameScene_->SceneInitialize();
		return;
	}
	if (color.w <= 0.0f) {
		frameCount_ = 0;
		color.w = 0.0f;
		isLoading_ = false;
		return;
	}
}

void SceneChange::Update2() {
	// フラグがfalseのときは早期return
	if (!isLoading_) {
		return;
	}

	frameCount_++;
	loadingSprite2_->SetPosition(loadingSprite2_->GetPosition() + Vector2{moveValue_, 0.0f});

	if (frameCount_ >= float(transitionFrame_ / 2.0f) && !isMoveIn_) {
		isMoveIn_ = true;
		*pScene_ = nextScene_;

		switch (nextScene_) {
		case SceneNum::TITLE:
			sound_->AllStopSound();
			sound_->OnPlayBGM(SoundManager::BGM::BGM_TITLE);
			break;
		case SceneNum::STAGE_SELECT:
			break;
		case SceneNum::TIME_ATTACK_STAGE:
			sound_->AllStopSound();
			//sound_->OnPlayBGM(SoundManager::BGM::BGM_TIME_ATTACK);
			break;
		case SceneNum::SCORE_ATTACK_STAGE:
			sound_->AllStopSound();
			//sound_->OnPlayBGM(SoundManager::BGM::BGM_TIME_ATTACK);
			break;
		default:
			sound_->AllStopSound();
			break;
		}
		gameScene_->SceneInitialize();
		return;
	}
	if (frameCount_ >= transitionFrame_ && isMoveIn_) {
		loadingSprite2_->SetPosition(Vector2{1280.0f, 0.0f});
		isMoveIn_ = false;
		frameCount_ = 0;
		isLoading_ = false;
		return;
	}
}

void SceneChange::Draw() {
	// フラグがfalseのときは早期return
	if (!isLoading_) {
		return;
	}

	loadingSprite_->Draw();
	loadingSprite2_->Draw();
}

SceneChange::SceneChange() {
	pScene_ = nullptr;
	nextScene_ = SceneNum::TITLE;
	sound_ = SoundManager::GetInstance();
	transitionFrame_ = 60;

	moveValue_ = -1280.0f / transitionFrame_ * 2.0f;
	isMoveIn_ = false;

	// 画像を読み込み、スプライトを生成する
	loadingTextureHandle_ = TextureManager::Load("now_loading.png");
	loadingSprite_ = Sprite::Create(loadingTextureHandle_, Vector2{0.0f, 0.0f});
	loadingSprite2_ = Sprite::Create(loadingTextureHandle_, Vector2{0.0f, 0.0f});
	loadingSprite_->SetColor(color);
	loadingSprite2_->SetPosition(Vector2{1280.0f, 0.0f});
	input_ = Input::GetInstance();
}

SceneChange::~SceneChange() {
	delete loadingSprite_;
	delete loadingSprite2_;
}
