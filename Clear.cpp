#include "Clear.h"
#include "SceneChange.h"
#include "TextureManager.h"

Clear::Clear() {
	// 画像を読み込み、スプライトを生成する
	clearTextureHandle_ = TextureManager::Load("clear.png");
	clearSprite_ = Sprite::Create(clearTextureHandle_, Vector2{0.0f, 0.0f});
}

Clear::~Clear() { delete clearSprite_; }

void Clear::Initialize(SceneNum* pScene) {
	input_ = Input::GetInstance();
	pScene_ = pScene;
}

void Clear::Update() {
	// SPACEでTITLEシーンに遷移する
	if (input_->PushKey(DIK_SPACE)) {
		SceneChange::GetInstance()->Change(SceneNum::TITLE, pScene_);
	}
}

void Clear::DrawBackground() { clearSprite_->Draw(); }
