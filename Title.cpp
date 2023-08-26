#include "Title.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "SceneChange.h"


Title::~Title() { delete titleSprite_; }

void Title::Initialize(SceneNum* pScene) {
	input_ = Input::GetInstance();
	pScene_ = pScene;

	//画像を読み込み、スプライトを生成する
	titleTextureHandle_ = TextureManager::Load("white1x1.png");
	titleSprite_ = Sprite::Create(titleTextureHandle_, Vector2{0.0f, 0.0f});
	titleSprite_->SetSize(Vector2{1280.0f, 720.0f});
}

void Title::Update() {
	//SPACEでSTAGEシーンに遷移する
	if (input_->PushKey(DIK_SPACE)) {
		SceneChange::GetInstance()->Change(SceneNum::STAGE, pScene_);
	}
	else if (input_->PushKey(DIK_RETURN)) {
		SceneChange::GetInstance()->Change(SceneNum::EXPLAIN, pScene_);
	}
	ImGui::Begin("a");
	ImGui::DragFloat4("a", &a.x, 0.01f);
	ImGui::End();
	titleSprite_->SetColor(a);
}

void Title::DrawBackground() {
	titleSprite_->Draw();
}
