#pragma once
#include "Collider.h"
#include "EnemyBullet.h"
#include "Model.h"
#include "TimeCall.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class Player;
class GameScene;
class ParticleSystem;

/// <summary>
/// エネミー
/// </summary>
class Enemy : public Collider {
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Enemy() {
		timeCalls_.clear();
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
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// 当たり判定
	/// </summary>
	void OnCollision();

	/// <summary>
	/// 座標のゲッター
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition() override;

	/// <summary>
	/// 半径のゲッター
	/// </summary>
	/// <returns></returns>
	const float& GetRadius() { return radius_; }

	/// <summary>
	/// デスフラグのゲッター
	/// </summary>
	/// <returns></returns>
	const bool& GetIsDead() { return isDead_; }

	/// <summary>
	/// プレイヤーのセッター
	/// </summary>
	/// <param name="player"></param>
	void SetPlayer(Player* player) { player_ = player; }

	/// <summary>
	/// ゲームシーンのセッター
	/// </summary>
	/// <param name="gameScene"></param>
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }
	
	/// <summary>
	/// パーティクルシステムのセッター
	/// </summary>
	/// <param name="gameScene"></param>
	void SetParticleSystem(ParticleSystem* particleSystem) { particleSystem_ = particleSystem; }

private:
	// 座標
	WorldTransform world_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// 半径
	float radius_ = 1.0f;

	Vector3 velocity_ = {};

	/// <summary>
	/// 攻撃の状態
	/// </summary>
	enum class AttackState { APPROACH, LEAVE };

	/// <summary>
	/// 接近状態の更新処理
	/// </summary>
	void ApproachUpdate();

	/// <summary>
	/// 離脱状態の更新処理
	/// </summary>
	void LeaveUpdate();

	// 攻撃
	AttackState state_ = AttackState::APPROACH;

	/// <summary>
	/// 攻撃処理
	/// </summary>
	void Attack();

	/// <summary>
	/// 弾を発射して発射クールタイムをリセットする
	/// </summary>
	void AttackReset();

	// デスフラグ
	bool isDead_ = false;
	// デスタイマー
	uint32_t deathTimer_ = 600;

	// 攻撃間隔
	int attackInterval = 45;

	// プレイヤー
	Player* player_ = nullptr;

	// ゲームシーン
	GameScene* gameScene_ = nullptr;

	//パーティクルシステム
	ParticleSystem* particleSystem_ = nullptr;

	// 時限発動のリスト
	list<TimeCall*> timeCalls_;
};
