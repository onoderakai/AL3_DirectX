﻿#pragma once
#include "Collider.h"
#include "WorldTransform.h"
#include "Model.h"
#include "BossBullet.h"
#include <list>

class ParticleSystem;
class Player;

class Boss : public Collider {
public:
	Boss();

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
	void Draw(const ViewProjection& view);

	/// <summary>
	/// 半径のゲッター
	/// </summary>
	/// <returns></returns>
	const float& GetRadius() override { return radius_; }

	/// <summary>
	/// 弾のゲッター
	/// </summary>
	/// <returns></returns>
	const std::list<BossBullet*>& GetBullets() { return bullets_; }

	/// <summary>
	/// 死亡フラグのゲッター
	/// </summary>
	/// <returns></returns>
	const bool& GetIsDead() { return isDead_; }

	/// <summary>
	/// ワールド座標のゲッター
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition() override;

	/// <summary>
	/// 衝突判定
	/// </summary>
	void OnCollision() override;

	/// <summary>
	/// パーティクルシステムのセッター
	/// </summary>
	/// <param name="particleSystem"></param>
	void SetParticleSystem(ParticleSystem* particleSystem) { particleSystem_ = particleSystem; }

	/// <summary>
	/// プレイヤーのセッター
	/// </summary>
	/// <param name="player"></param>
	void SetPlayer(Player* player) { player_ = player; }

private:
	ParticleSystem* particleSystem_ = nullptr;
	Player* player_ = nullptr;

	Model* model_ = nullptr;
	WorldTransform world_;
	float radius_ = 8.0f;

	//弾の包含
	Model* homingBulletModel_ = nullptr;
	std::list<BossBullet*> bullets_;

	//HP
	const uint32_t kMaxHp_ = 10;
	int32_t hp_ = kMaxHp_;
	bool isDead_ = false;

	void Attack();
};
