#pragma once
#include "Collider.h"
#include "WorldTransform.h"
#include "Model.h"

class Boss : public Collider {
public:
	~Boss();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection view);

private:
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0;
	WorldTransform world_;
};
