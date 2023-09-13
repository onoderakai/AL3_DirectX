#pragma once
#include "Model.h"
#include "WorldTransform.h"

class Particle {
public:
	enum class Type { CIRCLE, SPHERE, SCALE_CHANGE, RIGHT_PARTICLE, GATHER_PARTICLE };

	struct Parameter {
		// パーティクルの種類
		Type type_ = Type::SPHERE;
		// 座標
		WorldTransform world_;
		// 速度
		float speed_ = 1.0f;
		// デスタイム
		uint32_t deathTimer_ = 30;
	};

	Particle();

	void Initialize(const Parameter& parameter, const Vector3& velocity, Model* model);

	void Initialize(
	    const Parameter& parameter, const Vector3& velocity, Model* model, uint32_t textureHandle);

	void Update();

	void Draw(const ViewProjection& view);

	/// <summary>
	/// デスフラグのゲッター
	/// </summary>
	/// <returns></returns>
	bool GetIsDead() { return isDead_; }

	Vector3 GetWorldPosition();

private:
	// パラメーター
	Parameter parameter_ = {};
	Vector3 sizeChange = {};
	Vector3 velocity_ = {};
	Vector3 targetScale_ = {};
	float lerpAcceleration_ = 0.05f;

	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0;

	// パーティクルのデスフラグ
	bool isDead_ = false;

	void TypeCircleUpdate();

	void TypeSphereUpdate();

	void TypeScaleChangeUpdate();

	void TypeGatherUpdate();
};
