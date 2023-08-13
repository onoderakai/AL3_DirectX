#pragma once
#include "Model.h"
#include "WorldTransform.h"

class Particle {
public:
	enum class Type {
		CIRCLE,
		Sphere
	};

	void Initialize(const Vector3& pos, const Vector3& velocity,Type particleType, uint32_t textureHandle, Model* model);

	void Initialize(Model* model);

	void Update();

	void Draw(const ViewProjection& view);

private:
	//パーティクルの種類
	Type type_;

	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	//座標
	WorldTransform world_;

	//速度
	Vector3 velocity_ = {};

	void TypeCircleUpdate();

	void TypeSphereUpdate();
};
