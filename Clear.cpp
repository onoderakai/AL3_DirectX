#include "Clear.h"
#include "TextureManager.h"
#include "SceneChange.h"

Clear::~Clear() { delete clearSprite_; }

void Clear::Initialize(SceneNum* pScene) {
	input_ = Input::GetInstance();
	pScene_ = pScene;

	// 画像を読み込み、スプライトを生成する
	clearTextureHandle_ = TextureManager::Load("white1x1.png");
	clearSprite_ = Sprite::Create(clearTextureHandle_, Vector2{0.0f, 0.0f});
	clearSprite_->SetSize(Vector2{1280.0f, 720.0f});
	clearSprite_->SetColor(Vector4{1.0f, 1.0f, 1.0f, 1.0f});
}

void Clear::Update() {
	// SPACEでTITLEシーンに遷移する
	if (input_->PushKey(DIK_SPACE)) {
		SceneChange::GetInstance()->Change(SceneNum::TITLE, pScene_);
	}
}

void Clear::DrawBackground() { clearSprite_->Draw(); }
