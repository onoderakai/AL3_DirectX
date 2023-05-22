#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"

/// <summary>
/// エネミーバレット
/// </summary>
class EnemyBullet {
public:
	void Initialize(const Vector3& pos, const Vector3& velocity, Model* model);
	void Update();
	void Draw(ViewProjection& viewProjection);

	/// <summary>
	/// デスフラグのゲッター
	/// </summary>
	/// <returns></returns>
	bool GetIsDead() { return isDead_; }

private:
	WorldTransform world_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0;
	Vector3 velocity_;

	//弾の寿命
	const int kLifeTime = 60 * 2;
	//デスタイマー
	int deathTimer_ = kLifeTime;
	//デスフラグ
	bool isDead_ = false;
};
