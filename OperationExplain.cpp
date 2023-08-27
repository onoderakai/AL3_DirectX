#include "OperationExplain.h"
#include "TextureManager.h"
#include "SceneChange.h"

OperationExplain::OperationExplain() {
	// 画像を読み込み、スプライトを生成する
	explainTextureHandle_ = TextureManager::Load("explain.png");
	explainSprite_ = Sprite::Create(explainTextureHandle_, Vector2{0.0f, 0.0f});
}

OperationExplain::~OperationExplain() { delete explainSprite_; }

void OperationExplain::Initialize(SceneNum* pScene) {
	input_ = Input::GetInstance();
	pScene_ = pScene;
}

void OperationExplain::Update() {
	// SPACEでTITLEシーンに遷移する
	if (input_->PushKey(DIK_SPACE)) {
		SceneChange::GetInstance()->Change(SceneNum::TITLE, pScene_);
	}
}

void OperationExplain::DrawBackground() { explainSprite_->Draw(); }
