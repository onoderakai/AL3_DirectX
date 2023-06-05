#pragma once
#include "Model.h"
#include "WorldTransform.h"

class Skydome {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	void Initialize(Model* model);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection viewProjection);

private:
	WorldTransform world_;
	Model* model_ = nullptr;

};
