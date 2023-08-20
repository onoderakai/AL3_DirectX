#pragma once
#include "Collider.h"
#include "WorldTransform.h"
#include "Model.h"
#include "BossBullet.h"
#include <list>

class ParticleSystem;

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
	void Draw(ViewProjection view);

	/// <summary>
	/// 半径のゲッター
	/// </summary>
	/// <returns></returns>
	const float& GetRadius() override { return radius_; }

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

private:
	ParticleSystem* particleSystem_ = nullptr;

	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0;
	WorldTransform world_;
	float radius_ = 5.0f;

	//弾の包含
	Model* homingBulletModel_ = nullptr;
	std::list<BossBullet*> bullets_;

	//HP
	const uint32_t kMaxHp_ = 10;
	int32_t hp_ = kMaxHp_;
	bool isDead_ = false;
};
