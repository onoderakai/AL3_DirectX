#include "GameResult.h"
#include "SceneChange.h"

GameResult::GameResult() {
	input_ = Input::GetInstance();
	resultScore_ = new Score();
	sound_ = SoundManager::GetInstance();

	resultTex_[0] = TextureManager::Load("mode.png");
	resultTex_[1] = TextureManager::Load("result.png");
	resultTex_[2] = TextureManager::Load("score.png");

	// タイムアタックの画像
	resultTex_[3] = TextureManager::Load("time_attack.png");
	// スコアアタックの画像
	resultTex_[4] = TextureManager::Load("score_attack.png");

	// 背景画像
	uint32_t bgTex = TextureManager::Load("white1x1.png");
	// リトライ画像
	uint32_t pushNextTextureHandle = TextureManager::Load("A_retry.png");
	// タイトルに戻る画像
	uint32_t returnTitleTextureHandle = TextureManager::Load("B_return_title.png");

	// スプライトを生成
	backGround_ = Sprite::Create(bgTex, Vector2{0.0f, 0.0f}, Vector4{0.0f, 0.0f, 0.0f, 0.9f});
	backGround_->SetSize(Vector2{1280.0f, 720.0f});

	retrySprite_ = Sprite::Create(
	    pushNextTextureHandle, Vector2{640.0f - 200.0f, 620.0f}, {1, 1, 1, 1}, {0.5f, 0.5f});
	returnTitleSprite_ = Sprite::Create(
	    returnTitleTextureHandle, Vector2{640.0f + 200.0f, 620.0f}, {1, 1, 1, 1}, {0.5f, 0.5f});

	for (uint32_t i = 0; i < 4; i++) {
		resultSprite_[i] = Sprite::Create(resultTex_[i], Vector2{});
	}
}

GameResult::~GameResult() {
	delete resultScore_;
	for (uint32_t i = 0; i < 4; i++) {
		delete resultSprite_[i];
	}
	delete backGround_;
	delete returnTitleSprite_;
	delete retrySprite_;
}

void GameResult::Initialize(SceneNum* pScene) {
	pScene_ = pScene;
	isScoreDraw_ = true;
	isSound_ = false;
	drawCount_ = kMaxDrawCount_;
	resultScore_->Initialize();
}

void GameResult::Update() {
	// 接続状態を確認
	Input::GetInstance()->GetJoystickState(0, joyState_);
	Input::GetInstance()->GetJoystickStatePrevious(0, preJoyState_);

	// SPACEかAボタンで前のシーンに遷移する
	if (input_->TriggerKey(DIK_SPACE) || input_->TriggerKey(DIK_A) ||
	    (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A &&
	     (preJoyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A) == 0)) {
		sound_->OnPlaySound(SoundManager::Sound::SE_CONFIRM);
		SceneChange::GetInstance()->Change(preScene_, pScene_);
	}
	// RETURNかBボタンでTITLEシーンに遷移する
	else if (
	    input_->TriggerKey(DIK_RETURN) || input_->TriggerKey(DIK_B) ||
	    (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_B &&
	     (preJoyState_.Gamepad.wButtons & XINPUT_GAMEPAD_B) == 0)) {
		sound_->OnPlaySound(SoundManager::Sound::SE_CONFIRM);
		SceneChange::GetInstance()->Change(SceneNum::TITLE, pScene_);
	}

	isChange_ = SceneChange::GetInstance()->GetIsLoading();
	// シーン遷移中なら透明度を変えない
	if (isChange_) {
		retrySprite_->SetColor(Vector4{1.0f, 1.0f, 1.0f, 1.0f});
		returnTitleSprite_->SetColor(Vector4{1.0f, 1.0f, 1.0f, 1.0f});
		return;
	}
	// 透明度を徐々に変える
	Vector4 tmpColor = retrySprite_->GetColor();
	tmpColor.w += flashSpeed_;
	if (tmpColor.w <= 0.1f) {
		tmpColor.w = 0.1f;
		flashSpeed_ *= -1.0f;
	} else if (tmpColor.w >= 1.0f) {
		tmpColor.w = 1.0f;
		flashSpeed_ *= -1.0f;
	}
	retrySprite_->SetColor(tmpColor);
	returnTitleSprite_->SetColor(tmpColor);
}

void GameResult::TimeAttackDraw(const uint32_t& score) {
	resultSprite_[3]->SetTextureHandle(resultTex_[3]);
	// 音を鳴らす
	if (!isSound_) {
		isSound_ = true;
		sound_->OnPlaySound(SoundManager::Sound::SE_RESULT);
	}

	backGround_->Draw();
	for (uint32_t i = 0; i < 4; i++) {
		resultSprite_[i]->Draw();
	}

	resultScore_->DrawScoreTimeUI(score, isScoreDraw_);
	if (resultScore_->GetIsScoreDecision()) {
		if (isScoreDraw_) {
			drawCount_--;
		} else {
			drawCount_ -= 3;
		}
		if (drawCount_ <= 0) {
			drawCount_ = kMaxDrawCount_;
			isScoreDraw_ = !isScoreDraw_;
		}
		retrySprite_->Draw();
		returnTitleSprite_->Draw();
	}
}

void GameResult::ScoreAttackDraw(const uint32_t& score) {
	resultSprite_[3]->SetTextureHandle(resultTex_[4]);
	// 音を鳴らす
	if (!isSound_) {
		isSound_ = true;
		sound_->OnPlaySound(SoundManager::Sound::SE_RESULT);
	}

	backGround_->Draw();
	for (uint32_t i = 0; i < 4; i++) {
		resultSprite_[i]->Draw();
	}
	resultScore_->DrawScoreUIResult(score, isScoreDraw_);
	if (resultScore_->GetIsScoreDecision()) {
		if (isScoreDraw_) {
			drawCount_--;
		} else {
			drawCount_ -= 3;
		}
		if (drawCount_ <= 0) {
			drawCount_ = kMaxDrawCount_;
			isScoreDraw_ = !isScoreDraw_;
		}
		retrySprite_->Draw();
		returnTitleSprite_->Draw();
	}
}
