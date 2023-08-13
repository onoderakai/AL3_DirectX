#pragma once
#include "Input.h"
#include "Scene.h"

class Stage {

public:
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
	void Draw();

private:
	Input* input_ = nullptr;
	// シーン変数のポインタ
	SceneNum* pScene_;
};
