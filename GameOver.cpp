#include "GameOver.h"
#include "TextureManager.h"
#include "SceneChange.h"

GameOver::~GameOver() { delete gameOverSprite_; }

void GameOver::Initialize(SceneNum* pScene) {
	input_ = Input::GetInstance();
	pScene_ = pScene;

	// 画像を読み込み、スプライトを生成する
	gameOverTextureHandle_ = TextureManager::Load("white1x1.png");
	gameOverSprite_ = Sprite::Create(gameOverTextureHandle_, Vector2{0.0f, 0.0f});
	gameOverSprite_->SetSize(Vector2{1280.0f, 720.0f});
	gameOverSprite_->SetColor(Vector4{0.1f, 0.1f, 0.1f, 1.0f});
}

void GameOver::Update() {
	// SPACEでTITLEシーンに遷移する
	if (input_->PushKey(DIK_SPACE)) {
		SceneChange::GetInstance()->Change(SceneNum::TITLE, pScene_);
	}
}

void GameOver::DrawBackground() { gameOverSprite_->Draw(); }
