﻿#include "Title.h"
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
	uint32_t pushExplainTextureHandle = TextureManager::Load("push_explain.png");
	pushExplainSprite_ = Sprite::Create(
	    pushExplainTextureHandle, Vector2{640.0f, 600.0f}, {1, 1, 1, 1}, {0.5f, 0.5f});
}

Title::~Title() {
	delete titleSprite_;
	delete pushNextSprite_;
	delete pushExplainSprite_;
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

	isChange_ = SceneChange::GetInstance()->GetIsLoading();
	// シーン遷移中なら透明度を変えない
	if (isChange_) {
		pushNextSprite_->SetColor(Vector4{1.0f, 1.0f, 1.0f, 1.0f});
		pushExplainSprite_->SetColor(Vector4{1.0f, 1.0f, 1.0f, 1.0f});
		return;
	}
	// 透明度を徐々に変える
	Vector4 tmpColor = pushNextSprite_->GetColor();
	tmpColor.w += flashSpeed_;
	if (tmpColor.w <= 0.1f) {
		tmpColor.w = 0.1f;
		flashSpeed_ *= -1.0f;
	} else if (tmpColor.w >= 1.0f) {
		tmpColor.w = 1.0f;
		flashSpeed_ *= -1.0f;
	}
	pushNextSprite_->SetColor(tmpColor);
	pushExplainSprite_->SetColor(tmpColor);
}

void Title::DrawBackground() {
	titleSprite_->Draw();
	pushNextSprite_->Draw();
	pushExplainSprite_->Draw();
}
