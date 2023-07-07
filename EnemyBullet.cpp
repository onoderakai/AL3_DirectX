#include "EnemyBullet.h"
#include "MathUtility.h"
#include <cassert>

void EnemyBullet::Initialize(const Vector3& pos, const Vector3& velocity, Model* model) {
	assert(model);

	world_.Initialize();

	world_.scale_.x = 0.5f;
	world_.scale_.y = 0.5f;
	world_.scale_.z = 10.0f;

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

	world_.translation_ += velocity_;

	world_.UpdateMatrix();
}

void EnemyBullet::Draw(ViewProjection& viewProjection) {
	model_->Draw(world_, viewProjection, textureHandle_);
}

void EnemyBullet::OnCollision() {
	isDead_ = true;
}
