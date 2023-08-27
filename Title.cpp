#include "Title.h"
#include "ImGuiManager.h"
#include "SceneChange.h"
#include "TextureManager.h"

Title::Title() {
	// 画像を読み込み、スプライトを生成する
	titleTextureHandle_ = TextureManager::Load("title.png");
	titleSprite_ = Sprite::Create(titleTextureHandle_, Vector2{0.0f, 0.0f});

	uint32_t pushNextTextureHandle = TextureManager::Load("push_next.png");
	pushNextSprite_ =
	    Sprite::Create(pushNextTextureHandle, Vector2{640.0f, 480.0f}, {1, 1, 1, 1}, {0.5f, 0.5f});
}

Title::~Title() {
	delete titleSprite_;
	delete pushNextSprite_;
}

void Title::Initialize(SceneNum* pScene) {
	input_ = Input::GetInstance();
	pScene_ = pScene;
}

void Title::Update() {
	// SPACEでSTAGEシーンに遷移する
	if (input_->PushKey(DIK_SPACE)) {
		SceneChange::GetInstance()->Change(SceneNum::STAGE, pScene_);
	} else if (input_->PushKey(DIK_RETURN)) {
		SceneChange::GetInstance()->Change(SceneNum::EXPLAIN, pScene_);
	}
}

void Title::DrawBackground() {
	titleSprite_->Draw();
	pushNextSprite_->Draw();
}
