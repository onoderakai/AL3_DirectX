#pragma once
#include "Model.h"
#include "WorldTransform.h"

class Particle {
public:
	enum class Type {
		CIRCLE,
		SPHERE,
		SCALE_CHANGE
	};

	struct Parameter {
		// パーティクルの種類
		Type type_;
		// 座標
		WorldTransform world_;
		// 速度
		Vector3 velocity_ = {};
		//デスタイム
		uint32_t deathTimer_ = 60;
	};

	void Initialize(Parameter parameter, uint32_t textureHandle, Model* model);

	void Initialize(Parameter parameter,  Model* model);

	void Update();

	void Draw(const ViewProjection& view);

	/// <summary>
	/// デスフラグのゲッター
	/// </summary>
	/// <returns></returns>
	bool GetIsDead() { return isDead_; }

private:
	//パラメーター
	Parameter parameter_ = {};
	Vector3 sizeChange = {};

	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0;

	//パーティクルのデスフラグ
	bool isDead_ = false;

	void TypeCircleUpdate();

	void TypeSphereUpdate();

	void TypeScaleChangeUpdate();
};
