#include "OperationExplain.h"
#include "SceneChange.h"
#include "TextureManager.h"

OperationExplain::OperationExplain() {
	// 画像を読み込み、スプライトを生成する
	explainTextureHandle_ = TextureManager::Load("explain.png");
	explainSprite_ = Sprite::Create(explainTextureHandle_, Vector2{0.0f, 0.0f});

	uint32_t pushNextTextureHandle = TextureManager::Load("push_next.png");
	pushNextSprite_ =
	    Sprite::Create(pushNextTextureHandle, Vector2{640.0f, 660.0f}, {1, 1, 1, 1}, {0.5f, 0.5f});
}

OperationExplain::~OperationExplain() {
	delete explainSprite_;
	delete pushNextSprite_;
}

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

void OperationExplain::DrawBackground() {
	explainSprite_->Draw();
	pushNextSprite_->Draw();
}
