#include "PlayerBullet.h"
#include "MathUtility.h"
#include <cassert>

void PlayerBullet::Initialeze(Model* model, const Vector3& pos, const Vector3& velocity) {
	(assert(model));
	model_ = model;

	textureHandle_ = TextureManager::Load("white1x1.png");

	world_.Initialize();
	world_.translation_ = pos;

	world_.scale_.x = 0.5f;
	world_.scale_.y = 0.5f;
	world_.scale_.z = 2.0f;

	//行列計算で角度を変える
	Vector3 theta = velocity;
	theta = Normalize(theta);
	world_.rotation_.y = std::atan2f(theta.x, theta.z);
	Matrix4x4 rotY = MakeRotationYMatrix(-world_.rotation_.y);
	Vector3 velocityZ = TransformNormal(velocity, rotY);
	world_.rotation_.x = std::atan2f(-velocityZ.y, velocityZ.z);

	velocity_ = velocity;
}

void PlayerBullet::Update() {
	if (--deathTimer_ <= 0) {
		isDead_ = true;
		return;
	}

	world_.translation_ += velocity_;

	world_.UpdateMatrix();
}

void PlayerBullet::Draw(ViewProjection& viewProjection) {
	model_->Draw(world_, viewProjection, textureHandle_);
}

void PlayerBullet::OnCollision() {
	isDead_ = true; }

Vector3 PlayerBullet::GetWorldPosition() {
	Vector3 worldPos = {};
	worldPos = {world_.matWorld_.m[3][0], world_.matWorld_.m[3][1], world_.matWorld_.m[3][2]};
	return worldPos;
}
