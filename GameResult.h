#pragma once
#include "Sprite.h"
#include "TextureManager.h"
#include "Scene.h"
#include "Input.h"

class GameResult {
public:
	GameResult();
	~GameResult();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="pScene"></param>
	void Initialize(SceneNum* pScene);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// タイムアタックの描画
	/// </summary>
	void TimeAttackDraw();

	/// <summary>
	/// スコアアタックの描画
	/// </summary>
	void ScoreAttackDraw();

	/// <summary>
	/// 前のシーン情報のセッター
	/// </summary>
	/// <param name="preScene"></param>
	void SetPreScene(const SceneNum& preScene) { preScene_ = preScene; }

private:
	Input* input_ = nullptr;
	// ジョイスティックを使う
	XINPUT_STATE joyState_ = {};
	XINPUT_STATE preJoyState_ = {};

	// シーン変数のポインタ
	SceneNum* pScene_ = nullptr;
	// 前のシーンの情報
	SceneNum preScene_;

	//リザルト画面の画像
	uint32_t resultTex_[5] = {};
	//リザルト画面のスプライト
	Sprite* resultSprite_[4];
	//背景
	Sprite* backGround_ = nullptr;
};
