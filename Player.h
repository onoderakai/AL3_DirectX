#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "PlayerBullet.h"
#include "Sprite.h"
#include <list>
#include "Collider.h"

/// <summary>
/// 自キャラ
/// </summary>
class Player : public Collider{
public:

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
	void Initialeze(Model* model, uint32_t textureHandle, const Vector3& pos);

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
	void Draw(ViewProjection& viewProjection);

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
	/// 半径のゲッター
	/// </summary>
	/// <returns></returns>
	const float& GetRadius() { return radius_; }

	//セッター

	/// <summary>
	/// 親となるワールドトランスフォームをセット
	/// </summary>
	/// <param name="parent">親となるワールドトランスフォーム</param>
	void SetParent(const WorldTransform* parent) { world_.parent_ = parent;}

private:
	//入力
	Input* input_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// ワールド変換データ
	WorldTransform world_;
	// モデル情報
	Model* model_ = nullptr;
	//半径
	float radius_ = 1.0f;

	//弾
	std::list<PlayerBullet*> bullets_;
	//弾の発射クールタイム
	int bulletCoolTime_ = 0;

	//3Dレティクル
	WorldTransform world3DReticle_;
	WorldTransform world3DReticle2_;
	WorldTransform world3DReticle3_;
	//2Dレティクル用のスプライト
	Sprite* sprite2DReticle_ = nullptr;
	
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
};
