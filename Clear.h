#pragma once
#include "Scene.h"
#include "Input.h"
#include "Sprite.h"

class Clear {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Clear();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Clear();

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
	SceneNum* pScene_ = nullptr;

	// タイトル画像関連
	uint32_t clearTextureHandle_ = 0;
	Sprite* clearSprite_ = nullptr;

	// 次に進む
	Sprite* pushNextSprite_ = nullptr;
	bool isChange_ = false;
	float flashSpeed_ = 0.02f;
};
