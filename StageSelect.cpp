#include "StageSelect.h"
#include "SceneChange.h"
#include "TextureManager.h"

StageSelect::StageSelect() {
	easing_ = new Easing();

	// 画像を読み込み、スプライトを生成する
	uint32_t stageSelectBgTextureHandle_ = TextureManager::Load("stage_select_bg.png");
	stageSelectBgSprite_ = Sprite::Create(stageSelectBgTextureHandle_, Vector2{0.0f, 0.0f});
	uint32_t pushNextTextureHandle = TextureManager::Load("push_next.png");
	pushNextSprite_ = Sprite::Create(
	    pushNextTextureHandle, Vector2{}, {1, 1, 1, 1}, {0.5f, 0.5f});

	uint32_t stage1SelectTextureHandle = TextureManager::Load("stage1_select.png");
	stageSprite_[0] = Sprite::Create(
	    stage1SelectTextureHandle, Vector2{640.0f - 250, 380.0f}, {1, 1, 1, 1}, {0.5f, 0.5f});
	uint32_t bossSelectTextureHandle = TextureManager::Load("boss_stage_select.png");
	stageSprite_[1] = Sprite::Create(
	    bossSelectTextureHandle, Vector2{640.0f + 250, 380.0f}, {1, 1, 1, 1}, {0.5f, 0.5f});
	uint32_t selectTextureHandle = TextureManager::Load("select.png");
	selectSprite_ = Sprite::Create(
	    selectTextureHandle, stageSprite_[0]->GetPosition(), {1, 1, 1, 1}, {0.5f, 0.5f});

	uint32_t arrowRightTextureHandle = TextureManager::Load("select_arrow_right.png");
	arrowRightSprite_ = Sprite::Create(
	    arrowRightTextureHandle, Vector2{1280.0f - 170.0f, 380.0f}, {1, 1, 1, 1}, {0.5f, 0.5f});
	uint32_t arrowLeftTextureHandle = TextureManager::Load("select_arrow_left.png");
	arrowLeftSprite_ =
	    Sprite::Create(arrowLeftTextureHandle, Vector2{170.0f, 380.0f}, {1, 1, 1, 1}, {0.5f, 0.5f});

	uint32_t backTitleTextureHandle = TextureManager::Load("back_title.png");
	backTitleSprite_ =
	    Sprite::Create(backTitleTextureHandle, Vector2{80.0f, 80.0f}, {1, 1, 1, 1}, {0.5f, 0.5f});
}

StageSelect::~StageSelect() {
	delete easing_;
	delete stageSelectBgSprite_;
	for (uint32_t i = 0; i < maxStageNum_; i++) {
		delete stageSprite_[i];
	}
	delete selectSprite_;
	delete arrowRightSprite_;
	delete arrowLeftSprite_;
	delete pushNextSprite_;
	delete backTitleSprite_;
}

void StageSelect::Initialize(SceneNum* pScene) {
	selectSprite_->SetPosition(stageSprite_[0]->GetPosition());
	stageNum_ = 0;
	isEase_ = false;
	input_ = Input::GetInstance();
	pScene_ = pScene;
}

void StageSelect::Update() {
	// SPACEで選択したシーンに遷移する
	if (input_->PushKey(DIK_SPACE) && !isEase_) {
		if (stageNum_ == 0) {
			SceneChange::GetInstance()->Change(SceneNum::STAGE, pScene_);
		} else if (stageNum_ == 1) {
			SceneChange::GetInstance()->Change(SceneNum::BOSS_STAGE, pScene_);
		}
	}
	else if (input_->PushKey(DIK_ESCAPE)) {
		SceneChange::GetInstance()->Change(SceneNum::TITLE, pScene_);
	}

	if (!isEase_ && !SceneChange::GetInstance()->GetIsLoading()) {
		if ((input_->PushKey(DIK_RIGHT) || input_->PushKey(DIK_D)) &&
		    stageNum_ < maxStageNum_ - 1) {
			// イージングフラグをtrueにし、イージングの開始値と終了値を設定する
			isEase_ = true;
			start = stageSprite_[stageNum_]->GetPosition();
			stageNum_++;
			end = stageSprite_[stageNum_]->GetPosition();

		} else if ((input_->PushKey(DIK_LEFT) || input_->PushKey(DIK_A)) && stageNum_ > 0) {
			// イージングフラグをtrueにし、イージングの開始値と終了値を設定する
			isEase_ = true;
			start = stageSprite_[stageNum_]->GetPosition();
			stageNum_--;
			end = stageSprite_[stageNum_]->GetPosition();
		}
	}

	Vector2 selectPos = selectSprite_->GetPosition();
	selectPos = easing_->EaseOutSine(selectPos, start, end, 10, isEase_);
	selectSprite_->SetPosition(selectPos);
	pushNextSprite_->SetPosition(selectPos + Vector2{0.0f, 100.0f});
}

void StageSelect::DrawBackground() {
	stageSelectBgSprite_->Draw();
	arrowRightSprite_->Draw();
	arrowLeftSprite_->Draw();
	for (uint32_t i = 0; i < maxStageNum_; i++) {
		stageSprite_[i]->Draw();
	}
	selectSprite_->Draw();
	pushNextSprite_->Draw();
	backTitleSprite_->Draw();
}
