#pragma once
#include "Model.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Collider.h"

/// <summary>
/// プレイヤーの弾
/// </summary>
class PlayerBullet : public Collider {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="pos">初期座標</param>
	/// <param name="velocity">移動速度</param>
	void Initialeze(Model* model, const Vector3& pos, const Vector3& velocity);

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


	//ゲッター

	/// <summary>
	/// 弾のデスフラグのゲッター
	/// </summary>
	/// <returns></returns>
	const bool& GetIsDead() const { return isDead_; }

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
	//セッター


private:
	WorldTransform world_;
	Model* model_ = nullptr;
	
	Vector3 velocity_ = {};

	//弾の半径
	const float kRadius_ = 1.0f;
	//寿命(フレーム)
	static const int32_t kLifeTime = 60 * 5;
	//デスタイマー
	int32_t deathTimer_ = kLifeTime;
	//デスフラグ
	bool isDead_ = false;
};
