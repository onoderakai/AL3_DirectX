#pragma once
#include "Model.h"
#include "WorldTransform.h"

class Particle {
public:
	enum class Type { CIRCLE, SPHERE, SCALE_CHANGE };

	struct Parameter {
		// パーティクルの種類
		Type type_;
		// 座標
		WorldTransform world_;
		// 速度
		float speed_ = 1.0f;
		// デスタイム
		uint32_t deathTimer_ = 30;
	};

	Particle();

	void Initialize(
	    Parameter parameter, const Vector3& velocity, Model* model);

	void Initialize(
	    Parameter parameter, const Vector3& velocity, Model* model, uint32_t textureHandle);

	void Update();

	void Draw(const ViewProjection& view);

	/// <summary>
	/// デスフラグのゲッター
	/// </summary>
	/// <returns></returns>
	bool GetIsDead() { return isDead_; }

private:
	// パラメーター
	Parameter parameter_ = {};
	Vector3 sizeChange = {};
	Vector3 velocity_ = {};

	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0;

	// パーティクルのデスフラグ
	bool isDead_ = false;

	void TypeCircleUpdate();

	void TypeSphereUpdate();

	void TypeScaleChangeUpdate();
};
