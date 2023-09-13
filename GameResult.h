#pragma once
#include "Sprite.h"
#include "TextureManager.h"
#include "Scene.h"
#include "Input.h"
#include "Score.h"
#include "SoundManager.h"

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
	void TimeAttackDraw(const uint32_t& score);

	/// <summary>
	/// スコアアタックの描画
	/// </summary>
	void ScoreAttackDraw(const uint32_t& score);

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

	bool isSound_ = false;
	SoundManager* sound_ = nullptr;

	//スコア表示
	bool isScoreDraw_ = true;
	Score* resultScore_ = nullptr;
	const uint32_t kMaxDrawCount_ = 30;
	int32_t drawCount_ = kMaxDrawCount_;

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
	//タイトルに戻るボタン画像
	Sprite* returnTitleSprite_ = nullptr;
	//リトライボタン画像
	Sprite* retrySprite_ = nullptr;

	//シーン遷移中かどうかのフラグ
	bool isChange_ = false;

	//透明度の変わる速度
	float flashSpeed_ = -0.02f;
};
