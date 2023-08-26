#include "OperationExplain.h"
#include "TextureManager.h"
#include "SceneChange.h"

OperationExplain::~OperationExplain() { delete explainSprite_; }

void OperationExplain::Initialize(SceneNum* pScene) {
	input_ = Input::GetInstance();
	pScene_ = pScene;

	// 画像を読み込み、スプライトを生成する
	explainTextureHandle_ = TextureManager::Load("white1x1.png");
	explainSprite_ = Sprite::Create(explainTextureHandle_, Vector2{0.0f, 0.0f});
	explainSprite_->SetSize(Vector2{1280.0f, 720.0f});
	explainSprite_->SetColor(Vector4{1.0f, 1.0f, 1.0f, 1.0f});
}

void OperationExplain::Update() {
	// SPACEでTITLEシーンに遷移する
	if (input_->PushKey(DIK_SPACE)) {
		SceneChange::GetInstance()->Change(SceneNum::TITLE, pScene_);
	}
}

void OperationExplain::DrawBackground() { explainSprite_->Draw(); }
