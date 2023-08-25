﻿#pragma once
#include "Collider.h"
#include "EnemyType.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class Player;

/// <summary>
/// エネミーバレット
/// </summary>
class EnemyBullet : public Collider {
public:
	void Initialize(Type enemyType, const Vector3& pos, const Vector3& velocity, Model* model);
	void Update();
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// 当たり判定
	/// </summary>
	void OnCollision() override;

	/// <summary>
	/// デスフラグのゲッター
	/// </summary>
	/// <returns></returns>
	const bool& GetIsDead() { return isDead_; }

	/// <summary>
	/// 半径のゲッター
	/// </summary>
	/// <returns></returns>
	const float& GetRadius() override { return kRadius_; }

	/// <summary>
	/// 座標のゲッター
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition() override;

	/// <summary>
	/// プレイヤーのセッター
	/// </summary>
	/// <param name="player"></param>
	void SetPlayer(Player* player) { player_ = player; }

private:
	WorldTransform world_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0;
	Vector3 velocity_;

	// プレイヤーの包含
	Player* player_ = nullptr;

	//敵の種類
	Type bulletType_ = Type::NORMAL;

	// 弾の半径
	const float kRadius_ = 1.0f;
	// 弾の寿命
	const int kLifeTime = 60 * 20;
	// デスタイマー
	int deathTimer_ = kLifeTime;
	// デスフラグ
	bool isDead_ = false;

	void NormalUpdate();

	void ToPlayerUpdate();

	void HomingUpdate();
};
