#include "GameResult.h"
#include "SceneChange.h"

GameResult::GameResult() {
	input_ = Input::GetInstance();
	resultScore_ = new Score();

	resultTex_[0] = TextureManager::Load("mode.png");
	resultTex_[1] = TextureManager::Load("result.png");
	resultTex_[2] = TextureManager::Load("score.png");

	// タイムアタックの画像
	resultTex_[3] = TextureManager::Load("time_attack.png");
	// スコアアタックの画像
	resultTex_[4] = TextureManager::Load("score_attack.png");

	// 背景画像
	uint32_t bgTex = TextureManager::Load("white1x1.png");
	// 次に進む画像
	uint32_t pushNextTextureHandle = TextureManager::Load("push_next.png");

	// スプライトを生成
	backGround_ = Sprite::Create(bgTex, Vector2{0.0f, 0.0f}, Vector4{0.0f, 0.0f, 0.0f, 0.9f});
	backGround_->SetSize(Vector2{1280.0f, 720.0f});

	pushNextSprite_ =
	    Sprite::Create(pushNextTextureHandle, Vector2{640.0f, 620.0f}, {1, 1, 1, 1}, {0.5f, 0.5f});

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
	delete pushNextSprite_;
}

void GameResult::Initialize(SceneNum* pScene) {
	pScene_ = pScene;
	resultScore_->Initialize();
}

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

	isChange_ = SceneChange::GetInstance()->GetIsLoading();
	// シーン遷移中なら透明度を変えない
	if (isChange_) {
		pushNextSprite_->SetColor(Vector4{1.0f, 1.0f, 1.0f, 1.0f});
		return;
	}
	// 透明度を徐々に変える
	Vector4 tmpColor = pushNextSprite_->GetColor();
	tmpColor.w += flashSpeed_;
	if (tmpColor.w <= 0.1f) {
		tmpColor.w = 0.1f;
		flashSpeed_ *= -1.0f;
	} else if (tmpColor.w >= 1.0f) {
		tmpColor.w = 1.0f;
		flashSpeed_ *= -1.0f;
	}
	pushNextSprite_->SetColor(tmpColor);
}

void GameResult::TimeAttackDraw(const uint32_t& score) {
	resultSprite_[3]->SetTextureHandle(resultTex_[3]);

	backGround_->Draw();
	for (uint32_t i = 0; i < 4; i++) {
		resultSprite_[i]->Draw();
	}
	resultScore_->DrawScoreUIResult(score);
	if (resultScore_->GetIsScoreDecision()) {
		pushNextSprite_->Draw();
	}
}

void GameResult::ScoreAttackDraw(const uint32_t& score) {
	resultSprite_[3]->SetTextureHandle(resultTex_[4]);

	backGround_->Draw();
	for (uint32_t i = 0; i < 4; i++) {
		resultSprite_[i]->Draw();
	}
	resultScore_->DrawScoreUIResult(score);
	if (resultScore_->GetIsScoreDecision()) {
		pushNextSprite_->Draw();
	}
}
