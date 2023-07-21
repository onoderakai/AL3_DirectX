#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Collider.h"

class Player;

/// <summary>
/// エネミーバレット
/// </summary>
class EnemyBullet : public Collider {
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

	//プレイヤーの包含
	Player* player_ = nullptr;

	//弾の半径
	const float kRadius_ = 1.0f;
	//弾の寿命
	const int kLifeTime = 60 * 20;
	//デスタイマー
	int deathTimer_ = kLifeTime;
	//デスフラグ
	bool isDead_ = false;
};
