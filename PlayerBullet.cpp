#include "PlayerBullet.h"
#include "MathUtility.h"
#include "CollisionConfig.h"
#include <cassert>

void PlayerBullet::Initialeze(Model* model, const Vector3& pos, const Vector3& velocity) {
	(assert(model));
	model_ = model;

	world_.Initialize();
	world_.translation_ = pos;

	world_.scale_.x = 0.5f;
	world_.scale_.y = 0.5f;
	world_.scale_.z = 2.0f;

	world_.rotation_ = FaceToDirection(velocity);

	velocity_ = velocity;

	// 衝突フィルタリングを設定
	// このクラスの属性を設定
	SetCollisonAttribute(kCollisionAttributePlayer);
	// このクラスの衝突しない属性を設定
	SetCollisonMask(GetCollisionMask() ^ kCollisionAttributePlayer);
}

void PlayerBullet::Update() {
	if (--deathTimer_ <= 0) {
		isDead_ = true;
		return;
	}

	world_.translation_ += velocity_;

	world_.UpdateMatrix();
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(world_, viewProjection);
}

void PlayerBullet::OnCollision() {
	isDead_ = true; }

Vector3 PlayerBullet::GetWorldPosition() {
	Vector3 worldPos = {};
	worldPos = {world_.matWorld_.m[3][0], world_.matWorld_.m[3][1], world_.matWorld_.m[3][2]};
	return worldPos;
}
