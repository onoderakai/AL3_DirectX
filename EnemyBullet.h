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
	/// 当たり判定
	/// </summary>
	void OnCollision();
	

	/// <summary>
	/// デスフラグのゲッター
	/// </summary>
	/// <returns></returns>
	const bool& GetIsDead() { return isDead_; }

	/// <summary>
	/// 半径のゲッター
	/// </summary>
	/// <returns></returns>
	const float& GetRadius() { return kRadius_; }

	/// <summary>
	/// 座標のゲッター
	/// </summary>
	/// <returns></returns>
	const Vector3& GetWorldPosition() { return world_.translation_; }

private:
	WorldTransform world_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0;
	Vector3 velocity_;

	//弾の半径
	const float kRadius_ = 1.0f;
	//弾の寿命
	const int kLifeTime = 60 * 2;
	//デスタイマー
	int deathTimer_ = kLifeTime;
	//デスフラグ
	bool isDead_ = false;
};
