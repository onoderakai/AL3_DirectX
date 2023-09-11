#include "GameResult.h"
#include "SceneChange.h"

GameResult::GameResult() {
	input_ = Input::GetInstance();

	resultTex_[0] = TextureManager::Load("mode.png");
	resultTex_[1] = TextureManager::Load("result.png");
	resultTex_[2] = TextureManager::Load("score.png");

	//タイムアタックの画像
	resultTex_[3] = TextureManager::Load("time_attack.png");
	//スコアアタックの画像
	resultTex_[4] = TextureManager::Load("score_attack.png");

	//背景画像
	uint32_t bgTex = TextureManager::Load("white1x1.png");

	//スプライトを生成
	backGround_ = Sprite::Create(bgTex, Vector2{0.0f, 0.0f}, Vector4{0.0f, 0.0f, 0.0f, 0.9f});
	backGround_->SetSize(Vector2{1280.0f, 720.0f});

	for (uint32_t i = 0; i < 4; i++) {
		resultSprite_[i] = Sprite::Create(resultTex_[i], Vector2{});
	}
}

GameResult::~GameResult() {
	for (uint32_t i = 0; i < 4; i++) {
		delete resultSprite_[i];
	}
	delete backGround_;
}

void GameResult::Initialize(SceneNum* pScene) { pScene_ = pScene; }

void GameResult::Update() {
	// 接続状態を確認
	Input::GetInstance()->GetJoystickState(0, joyState_);
	Input::GetInstance()->GetJoystickStatePrevious(0, preJoyState_);

	// SPACEかAボタンで前のシーンに遷移する
	if (input_->PushKey(DIK_SPACE) || (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A &&
	                                   (preJoyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A) == 0)) {
		SceneChange::GetInstance()->Change(preScene_, pScene_);
	}
	// RETURNかBボタンでTITLEシーンに遷移する
	else if (
	    input_->PushKey(DIK_RETURN) || (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_B &&
	                                    (preJoyState_.Gamepad.wButtons & XINPUT_GAMEPAD_B) == 0)) {
		SceneChange::GetInstance()->Change(SceneNum::TITLE, pScene_);
	}
}

void GameResult::TimeAttackDraw() {
	resultSprite_[3]->SetTextureHandle(resultTex_[3]);
	
	backGround_->Draw();
	for (uint32_t i = 0; i < 4; i++) {
		resultSprite_[i]->Draw();
	}
}

void GameResult::ScoreAttackDraw() {
	resultSprite_[3]->SetTextureHandle(resultTex_[4]);

	backGround_->Draw();
	for (uint32_t i = 0; i < 4; i++) {
		resultSprite_[i]->Draw();
	}
}
