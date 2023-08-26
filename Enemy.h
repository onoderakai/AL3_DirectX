#pragma once
#include "Collider.h"
#include "EnemyBullet.h"
#include "Model.h"
#include "TimeCall.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "EnemyType.h"

class Player;
class GameScene;
class ParticleSystem;

/// <summary>
/// エネミー
/// </summary>
class Enemy : public Collider {
public:
	Enemy();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Enemy() { timeCalls_.clear(); }

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="pos">初期座標</param>
	void Initialize(Model* model, const Vector3& pos);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="type"></param>
	/// <param name="model"></param>
	/// <param name="pos"></param>
	void Initialize(Type type, Model* model, const Vector3& pos);

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
	void OnCollision() override;

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
	// 種類
	Type type_ = Type::NORMAL;

	// 座標
	WorldTransform world_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	uint32_t particleTextureHandle_ = 0;
	// 半径
	float radius_ = 1.0f;

	Vector3 velocity_ = {};

	// デスフラグ
	bool isDead_ = false;
	// デスタイマー
	uint32_t deathTimer_ = 600;

	//攻撃フラグ
	bool isAttack = false;
	// 攻撃間隔
	int attackInterval = 45;

	// プレイヤー
	Player* player_ = nullptr;

	// ゲームシーン
	GameScene* gameScene_ = nullptr;

	// パーティクルシステム
	ParticleSystem* particleSystem_ = nullptr;

	// 時限発動のリスト
	list<TimeCall*> timeCalls_;

	/// <summary>
	/// 種類に応じて弾を発射して発射クールタイムをリセットする
	/// </summary>
	void AttackReset();

	/// <summary>
	/// NORMALの更新処理
	/// </summary>
	void NormalUpdate();

	/// <summary>
	/// NORMALの更新処理
	/// </summary>
	void NormalAttack();

	/// <summary>
	/// TO_PLAYERの更新処理
	/// </summary>
	void ToPlayerUpdate();

	/// <summary>
	/// TO_PLAYERの更新処理
	/// </summary>
	void ToPlayerAttack();

	/// <summary>
	/// HOMINGの更新処理
	/// </summary>
	void HomingUpdate();

	/// <summary>
	/// 攻撃処理
	/// </summary>
	void HomingAttack();
};
