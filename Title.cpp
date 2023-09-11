#include "Title.h"
#include "ImGuiManager.h"
#include "SceneChange.h"
#include "TextureManager.h"

Title::Title() {
	// 画像を読み込み、スプライトを生成する
	titleTextureHandle_ = TextureManager::Load("title2.png");
	uint32_t titleBgTextureHandle_ = TextureManager::Load("title_bg.png");
	uint32_t titleDarumaTextureHandle_ = TextureManager::Load("title_daruma.png");
	titleSprite_ = Sprite::Create(
	    titleTextureHandle_, Vector2{640.0f, 240.0f}, Vector4{1.0f, 1.0f, 1.0f, 1.0f},
	    Vector2{0.5f, 0.5f});
	titleBgSprite_ = Sprite::Create(titleBgTextureHandle_, Vector2{0.0f, 0.0f});
	titleDarumaSprite_ = Sprite::Create(titleDarumaTextureHandle_, Vector2{0.0f, 0.0f});
	
	uint32_t pushNextTextureHandle = TextureManager::Load("push_next.png");
	pushNextSprite_ =
	    Sprite::Create(pushNextTextureHandle, Vector2{640.0f, 480.0f}, {1, 1, 1, 1}, {0.5f, 0.5f});
	uint32_t pushExplainTextureHandle = TextureManager::Load("push_explain.png");
	pushExplainSprite_ = Sprite::Create(
	    pushExplainTextureHandle, Vector2{640.0f, 600.0f}, {1, 1, 1, 1}, {0.5f, 0.5f});
}

Title::~Title() {
	delete titleSprite_;
	delete titleBgSprite_;
	delete pushNextSprite_;
	delete pushExplainSprite_;
	delete titleDarumaSprite_;
}

void Title::Initialize(SceneNum* pScene) {
	input_ = Input::GetInstance();
	pScene_ = pScene;
}

void Title::Update() {
	// 接続状態を確認
	Input::GetInstance()->GetJoystickState(0, joyState_);
	Input::GetInstance()->GetJoystickStatePrevious(0, preJoyState_);

	// SPACEかABXYボタンでSTAGE_SELECTシーンに遷移する
	if (input_->PushKey(DIK_SPACE) || (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A &&
	     (preJoyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A) == 0) ||
	    (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_B &&
	     (preJoyState_.Gamepad.wButtons & XINPUT_GAMEPAD_B) == 0) ||
	    (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_X &&
	     (preJoyState_.Gamepad.wButtons & XINPUT_GAMEPAD_X) == 0) ||
	    (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_Y &&
	     (preJoyState_.Gamepad.wButtons & XINPUT_GAMEPAD_Y) == 0)) {
		SceneChange::GetInstance()->Change(SceneNum::STAGE_SELECT, pScene_);
	} else if (input_->PushKey(DIK_RETURN)) {
		SceneChange::GetInstance()->Change(SceneNum::EXPLAIN, pScene_);
	}

	isChange_ = SceneChange::GetInstance()->GetIsLoading();
	// シーン遷移中なら透明度を変えない
	if (isChange_) {
		pushNextSprite_->SetColor(Vector4{1.0f, 1.0f, 1.0f, 1.0f});
		pushExplainSprite_->SetColor(Vector4{1.0f, 1.0f, 1.0f, 1.0f});
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
	pushExplainSprite_->SetColor(tmpColor);
}

void Title::DrawBackground() {
	titleBgSprite_->Draw();
	titleDarumaSprite_->Draw();
	titleSprite_->Draw();
	pushNextSprite_->Draw();
	pushExplainSprite_->Draw();
}
