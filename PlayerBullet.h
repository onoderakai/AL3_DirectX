#pragma once
#include "Model.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

/// <summary>
/// プレイヤーの弾
/// </summary>
class PlayerBullet {
public:
	void Initialeze(Model* model, const Vector3& pos, const Vector3& velocity);
	void Update();
	void Draw(ViewProjection& viewProjection);


	//ゲッター

	bool GetIsDead() const { return isDead_; }
	//セッター


private:
	WorldTransform world_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0;

	Vector3 velocity_ = {};

	//寿命(フレーム)
	static const int32_t kLifeTime = 60 * 5;
	//デスタイマー
	int32_t deathTimer_ = kLifeTime;
	//デスフラグ
	bool isDead_ = false;
};
