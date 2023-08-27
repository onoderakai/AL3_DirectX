#pragma once
#include "Input.h"
#include "Scene.h"
#include "Sprite.h"

class OperationExplain {
public:
	/// <summary>
	/// コンストラク
	/// </summary>
	OperationExplain();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~OperationExplain();

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

	// 操作説明画像関連
	uint32_t explainTextureHandle_ = 0;
	Sprite* explainSprite_ = nullptr;

	// 次に進む
	Sprite* pushNextSprite_ = nullptr;
	bool isChange_ = false;
	float flashSpeed_ = 0.02f;
};
