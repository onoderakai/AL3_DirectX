#pragma once
#include "Scene.h"
#include "Input.h"

class Title {
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
	SceneNum* pScene_;
};
