#pragma once
#include "Model.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

/// <summary>
/// プレイヤーの弾
/// </summary>
class PlayerBullet {
public:
	void Initialeze(Model* model, const Vector3& pos);
	void Update();
	void Draw(ViewProjection& viewProjection);

private:
	WorldTransform world_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0;
};
