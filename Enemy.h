#pragma once
#include "Model.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "EnemyBullet.h"
#include <list>
class Player;

/// <summary>
/// エネミー
/// </summary>
class Enemy {
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Enemy() {
		for (EnemyBullet* bullet : bullets_) {
			delete bullet;
		}
	}

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="pos">初期座標</param>
	void Initialeze(Model* model, const Vector3& pos);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(ViewProjection& viewProjection);

	/// <summary>
	/// 当たり判定
	/// </summary>
	void OnCollision();
	
	/// <summary>
	/// 弾のゲッター
	/// </summary>
	/// <returns></returns>
	const std::list<EnemyBullet*>& GetBullets() { return bullets_; }

	/// <summary>
	/// 座標のゲッター
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition() { return world_.translation_; }

	/// <summary>
	/// 半径のゲッター
	/// </summary>
	/// <returns></returns>
	float GetRadius() { return radius_; }

	/// <summary>
	/// プレイヤーのセッター
	/// </summary>
	/// <param name="player"></param>
	void SetPlayer(Player* player) { player_ = player; }

private:
	WorldTransform world_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0;
	float radius_ = 1.0f;

	Vector3 velocity_ = {};

	/// <summary>
	/// 攻撃の状態
	/// </summary>
	enum class AttackState {
		APPROACH,
		LEAVE
	};

	/// <summary>
	/// 接近状態の更新処理
	/// </summary>
	void ApproachUpdate();

	/// <summary>
	/// 離脱状態の更新処理
	/// </summary>
	void LeaveUpdate();

	//攻撃
	AttackState state_ = AttackState::APPROACH;

	/// <summary>
	/// 攻撃処理
	/// </summary>
	void Attack();

	//攻撃間隔
	int attackInterval = 45;
	//攻撃間隔をカウント
	int attackCount = 0;

	//弾の包含
	std::list<EnemyBullet*> bullets_;
	//プレイヤー
	Player* player_ = nullptr;
};
