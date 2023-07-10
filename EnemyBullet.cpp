#include "EnemyBullet.h"
#include "MathUtility.h"
#include <cassert>
#include "Player.h"

void EnemyBullet::Initialize(const Vector3& pos, const Vector3& velocity, Model* model) {
	assert(model);

	world_.Initialize();

	world_.scale_.x = 0.5f;
	world_.scale_.y = 0.5f;
	world_.scale_.z = 2.0f;

	//行列計算をしないで回転
	Vector3 theta = velocity;
	theta = Normalize(theta);
	world_.rotation_.y = std::atan2f(theta.x, theta.z);
	float width = sqrtf(theta.x * theta.x + theta.z * theta.z);
	world_.rotation_.x = std::atan2f(-theta.y, width);
	
	velocity_ = velocity;
	world_.translation_ = pos;
	model_ = model;
	textureHandle_ = TextureManager::Load("white1x1.png");
}

void EnemyBullet::Update() {
	if (--deathTimer_ <= 0) {
		isDead_ = true;
		return;
	}

	// ホーミング
	Vector3 toPlayer = player_->GetWorldPosition() - GetWorldPosition();
	// ベクトルを正規化する
	toPlayer = Normalize(toPlayer);
	velocity_ = Normalize(velocity_);
	// 球面線形補間により、今の速度と自キャラへのベクトルを内挿し、新たな速度とする
	velocity_ = Lerp(velocity_, toPlayer, 0.2f) * 1.0f;

	// 行列計算をしないで回転/////////////////////////////
	Vector3 theta = velocity_;
	theta = Normalize(theta);
	world_.rotation_.y = std::atan2f(theta.x, theta.z);
	float width = sqrtf(theta.x * theta.x + theta.z * theta.z);
	world_.rotation_.x = std::atan2f(-theta.y, width);
	////////////////////////////////////////////////////

	world_.translation_ += velocity_;

	world_.UpdateMatrix();
}

void EnemyBullet::Draw(ViewProjection& viewProjection) {
	model_->Draw(world_, viewProjection, textureHandle_);
}

void EnemyBullet::OnCollision() {
	//isDead_ = true;
}

Vector3 EnemyBullet::GetWorldPosition() {
	Vector3 worldPos = {};
	worldPos = {world_.matWorld_.m[3][0], world_.matWorld_.m[3][1], world_.matWorld_.m[3][2]};
	return worldPos;
}
