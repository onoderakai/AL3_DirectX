#pragma once
#include "Collider.h"
#include "Model.h"
#include "WorldTransform.h"

class Player;

class BossBullet : public Collider {
public:
	enum class AttackType { NORMAL, HOMING };
	void Initialize(Model* model);
	void Initialize(Model* model, const float& speed);
	void Update();
	void Draw(const ViewProjection& view);

	/// <summary>
	/// 当たり判定
	/// </summary>
	void OnCollision() override;

	/// <summary>
	/// デスフラグのゲッター
	/// </summary>
	/// <returns></returns>
	bool GetIsDead() { return isDead_; }

	/// <summary>
	/// 座標のゲッター
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition() override;

	/// <summary>
	/// 半径のゲッター
	/// </summary>
	/// <returns></returns>
	const float& GetRadius() override { return radius_; }

	// セッター

	/// <summary>
	/// プレイヤーのセッター
	/// </summary>
	/// <param name="player"></param>
	void SetPlayer(Player* player) { player_ = player; }

private:
	//弾の種類
	AttackType attackType_ = AttackType::HOMING;

	Player* player_ = nullptr;
	Model* model_ = nullptr;
	WorldTransform world_;

	float radius_ = 5.0f;

	// HP
	const uint32_t kMaxHp_ = 600;
	int32_t hp_ = kMaxHp_;
	// デスフラグ
	bool isDead_ = false;

	Vector3 velocity_ = {};
	float speed_ = 0.2f;

	/// <summary>
	/// 通常攻撃の更新処理
	/// </summary>
	void AttackNormalUpdate();

	/// <summary>
	/// ホーミング攻撃の更新処理
	/// </summary>
	void AttackHomingUpdate();
};
