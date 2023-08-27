#include "GameOver.h"
#include "TextureManager.h"
#include "SceneChange.h"

GameOver::GameOver() {
	preScene_ = SceneNum::TITLE;
	// 画像を読み込み、スプライトを生成する
	gameOverTextureHandle_ = TextureManager::Load("gameover.png");
	gameOverSprite_ = Sprite::Create(gameOverTextureHandle_, Vector2{0.0f, 0.0f});
}

GameOver::~GameOver() { delete gameOverSprite_; }

void GameOver::Initialize(SceneNum* pScene) {
	input_ = Input::GetInstance();
	pScene_ = pScene;
}

void GameOver::Update() {
	// SPACEで前のシーンに遷移する
	if (input_->PushKey(DIK_SPACE)) {
		SceneChange::GetInstance()->Change(preScene_, pScene_);
	}
	// RETURNでTITLEシーンに遷移する
	else if (input_->PushKey(DIK_RETURN)) {
		SceneChange::GetInstance()->Change(SceneNum::TITLE, pScene_);
	}
}

void GameOver::DrawBackground() { gameOverSprite_->Draw(); }
