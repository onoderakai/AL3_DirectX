#include "Title.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "SceneChange.h"


Title::Title() {
	// 画像を読み込み、スプライトを生成する
	titleTextureHandle_ = TextureManager::Load("title.png");
	titleSprite_ = Sprite::Create(titleTextureHandle_, Vector2{0.0f, 0.0f});
}

Title::~Title() { delete titleSprite_; }

void Title::Initialize(SceneNum* pScene) {
	input_ = Input::GetInstance();
	pScene_ = pScene;
}

void Title::Update() {
	//SPACEでSTAGEシーンに遷移する
	if (input_->PushKey(DIK_SPACE)) {
		SceneChange::GetInstance()->Change(SceneNum::STAGE, pScene_);
	}
	else if (input_->PushKey(DIK_RETURN)) {
		SceneChange::GetInstance()->Change(SceneNum::EXPLAIN, pScene_);
	}
}

void Title::DrawBackground() {
	titleSprite_->Draw();
}
