#include "StageSelect.h"
#include "SceneChange.h"
#include "TextureManager.h"

StageSelect::StageSelect() {
	easing_ = new Easing();
	sound_ = SoundManager::GetInstance();

	// 画像を読み込み、スプライトを生成する
	uint32_t stageSelectBgTextureHandle_ = TextureManager::Load("title_bg.png");
	stageSelectBgSprite_ = Sprite::Create(stageSelectBgTextureHandle_, Vector2{0.0f, 0.0f});
	uint32_t pushNextTextureHandle = TextureManager::Load("A_push.png");
	pushNextSprite_ = Sprite::Create(pushNextTextureHandle, Vector2{}, {1, 1, 1, 1}, {0.5f, 0.5f});
	// 操作説明を開くための画像
	uint32_t pushExplainTextureHandle = TextureManager::Load("X_explain.png");
	pushExplainSprite_ = Sprite::Create(
	    pushExplainTextureHandle, Vector2{200.0f, 80.0f}, {1, 1, 1, 1}, {0.5f, 0.5f});
	// 操作説明画像
	uint32_t explain1TextureHandle = TextureManager::Load("left_explain.png");
	explainSprite_[0] = Sprite::Create(
	    explain1TextureHandle, Vector2{640.0f, 360.0f - 160.0f}, {1, 1, 1, 1}, {0.5f, 0.5f});
	uint32_t explain2TextureHandle = TextureManager::Load("right_explain.png");
	explainSprite_[1] = Sprite::Create(
	    explain2TextureHandle, Vector2{640.0f, 360.0f + 140.0f}, {1, 1, 1, 1}, {0.5f, 0.5f});
	uint32_t explain3TextureHandle = TextureManager::Load("explain_cat.png");
	explainSprite_[2] =
	    Sprite::Create(explain3TextureHandle, Vector2{640.0f, 350.0f}, {1, 1, 1, 1}, {0.5f, 0.5f});
	// 操作説明背景
	uint32_t bgTex = TextureManager::Load("white1x1.png");
	backGround_ = Sprite::Create(bgTex, Vector2{0.0f, 0.0f}, Vector4{0.0f, 0.0f, 0.0f, 0.9f});
	backGround_->SetSize(Vector2{1280.0f, 720.0f});

	uint32_t stage1SelectTextureHandle = TextureManager::Load("stage1_select.png");
	stageSprite_[0] = Sprite::Create(
	    stage1SelectTextureHandle, Vector2{640.0f - 250, 380.0f}, {1, 1, 1, 1}, {0.5f, 0.5f});
	uint32_t bossSelectTextureHandle = TextureManager::Load("boss_stage_select.png");
	stageSprite_[1] = Sprite::Create(
	    bossSelectTextureHandle, Vector2{640.0f + 250, 380.0f}, {1, 1, 1, 1}, {0.5f, 0.5f});
	uint32_t selectTextureHandle = TextureManager::Load("select.png");
	selectSprite_ = Sprite::Create(
	    selectTextureHandle, stageSprite_[0]->GetPosition(), {1, 1, 1, 1}, {0.5f, 0.5f});

	uint32_t arrowRightTextureHandle = TextureManager::Load("RB.png");
	arrowRightSprite_ = Sprite::Create(
	    arrowRightTextureHandle, Vector2{1280.0f - 130.0f, 380.0f}, {1, 1, 1, 1}, {0.5f, 0.5f});
	uint32_t arrowLeftTextureHandle = TextureManager::Load("LB.png");
	arrowLeftSprite_ =
	    Sprite::Create(arrowLeftTextureHandle, Vector2{130.0f, 380.0f}, {1, 1, 1, 1}, {0.5f, 0.5f});
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
	delete pushExplainSprite_;
	for (uint32_t i = 0; i < 3; i++) {
		delete explainSprite_[i];
	}
	delete backGround_;
}

void StageSelect::Initialize(SceneNum* pScene) {
	selectSprite_->SetPosition(stageSprite_[0]->GetPosition());
	stageNum_ = 0;
	isExplain_ = false;
	isEase_ = false;
	input_ = Input::GetInstance();
	pScene_ = pScene;
}

void StageSelect::Update() {
	// 接続状態を確認
	Input::GetInstance()->GetJoystickState(0, joyState_);
	Input::GetInstance()->GetJoystickStatePrevious(0, preJoyState_);

	// SPACEかAボタンで選択したシーンに遷移する
	if (!isExplain_) {
		if ((input_->TriggerKey(DIK_SPACE) || input_->TriggerKey(DIK_A) ||
		     (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A &&
		      (preJoyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A) == 0)) &&
		    !isEase_) {
			sound_->OnPlaySound(SoundManager::Sound::SE_CONFIRM);
			if (stageNum_ == 0) {
				SceneChange::GetInstance()->Change(SceneNum::TIME_ATTACK_STAGE, pScene_);
			} else if (stageNum_ == 1) {
				SceneChange::GetInstance()->Change(SceneNum::SCORE_ATTACK_STAGE, pScene_);
			}
		} else if (input_->PushKey(DIK_ESCAPE)) {
			SceneChange::GetInstance()->Change(SceneNum::TITLE, pScene_);
		}
	}

	// XボタンかENTERで操作説明を開く
	if (!isExplain_ && (input_->TriggerKey(DIK_RETURN) || input_->TriggerKey(DIK_X) ||
	                    (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_X &&
	                     (preJoyState_.Gamepad.wButtons & XINPUT_GAMEPAD_X) == 0))) {
		sound_->OnPlaySound(SoundManager::Sound::SE_EXPLAIN);
		isExplain_ = true;
	} else if (
	    isExplain_ && (input_->TriggerKey(DIK_RETURN) || input_->TriggerKey(DIK_X) ||
	                   (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A &&
	                    (preJoyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A) == 0) ||
	                   (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_B &&
	                    (preJoyState_.Gamepad.wButtons & XINPUT_GAMEPAD_B) == 0) ||
	                   (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_X &&
	                    (preJoyState_.Gamepad.wButtons & XINPUT_GAMEPAD_X) == 0) ||
	                   (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_Y &&
	                    (preJoyState_.Gamepad.wButtons & XINPUT_GAMEPAD_Y) == 0))) {
		sound_->OnPlaySound(SoundManager::Sound::SE_EXPLAIN);
		isExplain_ = false;
	}

	if (!isEase_ && !SceneChange::GetInstance()->GetIsLoading()) {
		if ((input_->PushKey(DIK_RIGHT) ||
		     (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER &&
		      (preJoyState_.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) == 0)) &&
		    stageNum_ < maxStageNum_ - 1) {
			sound_->OnPlaySound(SoundManager::Sound::SE_SELECT_RB_LB);
			// イージングフラグをtrueにし、イージングの開始値と終了値を設定する
			isEase_ = true;
			start = stageSprite_[stageNum_]->GetPosition();
			stageNum_++;
			end = stageSprite_[stageNum_]->GetPosition();

		} else if (
		    (input_->PushKey(DIK_LEFT) ||
		     (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER &&
		      (preJoyState_.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) == 0)) &&
		    stageNum_ > 0) {
			sound_->OnPlaySound(SoundManager::Sound::SE_SELECT_RB_LB);
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
	pushNextSprite_->SetPosition(selectPos + Vector2{0.0f, 200.0f});
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
	pushExplainSprite_->Draw();

	// 説明画面が出てるときだけ
	if (isExplain_) {
		backGround_->Draw();
		for (uint32_t i = 0; i < 3; i++) {
			explainSprite_[i]->Draw();
		}
	}
}
