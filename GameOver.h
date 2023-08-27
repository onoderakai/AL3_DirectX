#pragma once
#include "ImGuiManager.h"
#include "Input.h"
#include "Scene.h"
#include "Sprite.h"

class GameOver {
public:
	/// <summary>
	/// コンストラク
	/// </summary>
	GameOver();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameOver();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(SceneNum* pScene);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void DrawBackground();

	/// <summary>
	/// 前のシーン情報のセッター
	/// </summary>
	/// <param name="preScene"></param>
	void SetPreScene(const SceneNum& preScene) { preScene_ = preScene; }

private:
	Input* input_ = nullptr;
	// シーン変数のポインタ
	SceneNum* pScene_ = nullptr;
	// 前のシーンの情報
	SceneNum preScene_;

	// タイトル画像関連
	uint32_t gameOverTextureHandle_ = 0;
	Sprite* gameOverSprite_ = nullptr;

	//リトライ文字
	Sprite* retrySprite_ = nullptr;
	// タイトルに戻る文字
	Sprite* returnTitleSprite_ = nullptr;
};
