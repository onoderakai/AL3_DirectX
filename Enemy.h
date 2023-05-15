#pragma once
#include "Model.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

/// <summary>
/// エネミー
/// </summary>
class Enemy {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="pos">初期座標</param>
	void Initialeze(Model* model, const Vector3& pos);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(ViewProjection& viewProjection);

private:
	WorldTransform world_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0;

	Vector3 velocity_ = {};
};
