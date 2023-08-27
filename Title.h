#pragma once
#include "Scene.h"
#include "Input.h"
#include "Sprite.h"

class Title {
public:
	/// <summary>
	/// コンストラク
	/// </summary>
	Title();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Title();

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
	//シーン変数のポインタ
	SceneNum* pScene_ = nullptr;

	//タイトル画像関連
	uint32_t titleTextureHandle_ = 0;
	Sprite* titleSprite_ = nullptr;
};
