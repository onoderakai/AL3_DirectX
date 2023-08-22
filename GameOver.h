#pragma once
#include "Input.h"
#include "Scene.h"
#include "Sprite.h"

class GameOver {
public:
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

private:
	Input* input_ = nullptr;
	// シーン変数のポインタ
	SceneNum* pScene_;

	// タイトル画像関連
	uint32_t gameOverTextureHandle_ = 0;
	Sprite* gameOverSprite_ = nullptr;
};
