#include "GameOver.h"
#include "SceneChange.h"
#include "TextureManager.h"

GameOver::GameOver() {
	preScene_ = SceneNum::TITLE;
	// 画像を読み込み、スプライトを生成する
	gameOverTextureHandle_ = TextureManager::Load("gameover.png");
	gameOverSprite_ = Sprite::Create(gameOverTextureHandle_, Vector2{0.0f, 0.0f});

	uint32_t retryTextureHandle = TextureManager::Load("push_retry.png");
	retrySprite_ =
	    Sprite::Create(retryTextureHandle, Vector2{640.0f, 480.0f}, {1, 1, 1, 1}, {0.5f, 0.5f});

	uint32_t returnTitleTextureHandle = TextureManager::Load("push_title.png");
	returnTitleSprite_ = Sprite::Create(
	    returnTitleTextureHandle, Vector2{640.0f, 600.0f}, {1, 1, 1, 1}, {0.5f, 0.5f});
}

GameOver::~GameOver() {
	delete gameOverSprite_;
	delete retrySprite_;
	delete returnTitleSprite_;
}

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

void GameOver::DrawBackground() {
	gameOverSprite_->Draw();
	retrySprite_->Draw();
	returnTitleSprite_->Draw();
}
