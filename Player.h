#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "PlayerBullet.h"
#include "Sprite.h"
#include <list>
#include "Collider.h"

class Enemy;
class ParticleSystem;

/// <summary>
/// 自キャラ
/// </summary>
class Player : public Collider{
public:
	enum class Style {
		NORMAL,
		SNIPER
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Player();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル情報</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	/// <param name="pos">生成座標</param>
	void Initialeze(Model* model, Model* sniperModel, const Vector3& pos);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(const ViewProjection& viewProjection);

	/// <summary>
	/// 移動処理
	/// </summary>
	/// <param name="joyState"></param>
	void Move(XINPUT_STATE& joyState);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// UIを表示する
	/// </summary>
	void DrawUI();

	/// <summary>
	/// 当たり判定
	/// </summary>
	void OnCollision() override;
	
	//ゲッター
	
	/// <summary>
	/// 死亡フラグのゲッター
	/// </summary>
	/// <returns></returns>
	const bool& GetIsDead() { return isDead_; }

	/// <summary>
	/// 弾のゲッター
	/// </summary>
	/// <returns></returns>
	const std::list<PlayerBullet*>& GetBullets() { return bullets_; }

	/// <summary>
	/// 座標のゲッター
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition() override;

	/// <summary>
	/// 3Dレティクルのワールド座標を取得
	/// </summary>
	/// <returns></returns>
	Vector3 Get3DReticleWorldPosition();

	/// <summary>
	/// 3Dレティクルのワールド座標を取得
	/// </summary>
	/// <returns></returns>
	Vector3 Get3DReticleFrontWorldPosition();

	/// <summary>
	/// 3Dレティクルのワールド座標を取得
	/// </summary>
	/// <returns></returns>
	Vector3 Get3DReticleBackWorldPosition();

	/// <summary>
	/// 半径のゲッター
	/// </summary>
	/// <returns></returns>
	const float& GetRadius() override { return radius_; }

	//セッター

	/// <summary>
	/// 親となるワールドトランスフォームをセット
	/// </summary>
	/// <param name="parent">親となるワールドトランスフォーム</param>
	void SetParent(const WorldTransform* parent) { world_.parent_ = parent;}

	/// <summary>
	/// 敵のセッター
	/// </summary>
	/// <param name="enemys"></param>
	void SetEnemys(std::list<Enemy*> enemys) { enemys_ = enemys; }

	/// <summary>
	/// パーティクルシステムのセッター
	/// </summary>
	/// <param name="particleSystem"></param>
	void SetParticleSystem(ParticleSystem* particleSystem) { particleSystem_ = particleSystem; }

private:
	Style style_ = Style::NORMAL;
	//入力
	Input* input_ = nullptr;
	// ワールド変換データ
	WorldTransform world_;
	// モデル情報
	Model* model_ = nullptr;
	Model* sniperModel_ = nullptr;
	Model* bulletModel_ = nullptr;
	//半径
	float radius_ = 2.5f;
	//デスフラグ
	bool isDead_ = false;
	//HP
	const uint32_t kMaxHp_ = 10;
	Sprite* hpSprite_[10] = {};
	int32_t hp_ = kMaxHp_;

	//敵のリスト
	std::list<Enemy*> enemys_;
	//パーティクルシステム
	ParticleSystem* particleSystem_ = nullptr;

	//ロックオンフラグ
	bool isLockOn = false;
	//ロックオン座標
	Vector3 lockOnPos = {};
	//ロックオンする距離
	float lockOnDis = 100.0f;
	//一番小さい数値を保存
	float nearDis = lockOnDis;
	//ロックオンした敵の情報
	Enemy* lockOnEnemy_ = nullptr;

	//弾
	std::list<PlayerBullet*> bullets_;
	//弾の発射クールタイム
	int bulletCoolTime_ = 0;
	const uint32_t kNormalCoolTime_ = 5;
	const uint32_t kSniperCoolTime_ = 60;

	//弾の速度
	const float kNormalBulletSpeed_ = 5.0f;
	const float kSniperBulletSpeed_ = 20.0f;

	//3Dレティクル
	WorldTransform world3DReticle_;
	WorldTransform world3DReticleBack_;
	WorldTransform world3DReticleFront_;
	//2Dレティクル用のスプライト
	Sprite* sprite2DReticle_ = nullptr;
	Sprite* sprite2DReticleBack_ = nullptr;
	Sprite* sprite2DReticleFront_ = nullptr;
	
	/// <summary>
	/// 形態変化
	/// </summary>
	void StyleChange();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	/// <summary>
	/// 3Dレティクルの座標をプレイヤーに追従させる
	/// </summary>
	void Trans3DReticle();

	/// <summary>
	/// UI画像をワールド座標からマウスのスクリーン座標に変換する
	/// </summary>
	/// <param name="viewProjection"></param>
	void WorldToScreen2DReticle(const ViewProjection& viewProjection);

	/// <summary>
	/// 3Dモデルの座標をUI画像の位置に移動させる(UI画像はキーボード、ジョイスティックで移動できる)
	/// </summary>
	/// <param name="viewProjection"></param>
	void ScreenToWorld2DReticle(const ViewProjection& viewProjection, XINPUT_STATE& joyState);

	/// <summary>
	/// HPの描画
	/// </summary>
	void HpDraw();
};
