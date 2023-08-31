#include "EnemyBullet.h"
#include "CollisionConfig.h"
#include "MathUtility.h"
#include "Player.h"
#include <cassert>

void EnemyBullet::Initialize(
    Type enemyType, const Vector3& pos, const Vector3& velocity, Model* model) {
	assert(model);

	bulletType_ = enemyType;

	world_.Initialize();

	world_.scale_.x = 0.5f;
	world_.scale_.y = 0.5f;
	world_.scale_.z = 2.0f;

	world_.rotation_ = FaceToDirection(velocity);

	speed_ = Length(velocity);
	velocity_ = velocity;
	world_.translation_ = pos;
	model_ = model;

	// 衝突フィルタリングを設定
	// このクラスの属性を設定
	SetCollisonAttribute(kCollisionAttributeBullet);
	// このクラスの衝突しない属性を設定
	SetCollisonMask(GetCollisionMask() ^ kCollisionAttributeEnemy ^ kCollisionAttributeBullet);
}

void EnemyBullet::Update() {
	if (--deathTimer_ <= 0) {
		isDead_ = true;
		return;
	}

	switch (bulletType_) {
	case Type::NORMAL:
		NormalUpdate();
		break;
	case Type::TO_PLAYER:
		ToPlayerUpdate();
		break;
	case Type::HOMING:
		HomingUpdate();
		break;
	default:
		break;
	}

	world_.translation_ += velocity_;

	world_.UpdateMatrix();
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(world_, viewProjection);
}

void EnemyBullet::OnCollision() { isDead_ = true; }

Vector3 EnemyBullet::GetWorldPosition() {
	Vector3 worldPos = {};
	worldPos = {world_.matWorld_.m[3][0], world_.matWorld_.m[3][1], world_.matWorld_.m[3][2]};
	return worldPos;
}

void EnemyBullet::NormalUpdate() {}

void EnemyBullet::ToPlayerUpdate() {}

void EnemyBullet::HomingUpdate() {
	if (!isHoming_) {
		return;
	}

	float homingLength = 10.0f;
	if (Length(player_->GetWorldPosition() - GetWorldPosition()) >= homingLength) {
		// ホーミング
		Vector3 toPlayer = player_->GetWorldPosition() - GetWorldPosition();
		// ベクトルを正規化する
		toPlayer = Normalize(toPlayer);
		velocity_ = Normalize(velocity_);
		// 球面線形補間により、今の速度と自キャラへのベクトルを内挿し、新たな速度とする
		velocity_ = Lerp(velocity_, toPlayer, 0.05f) * speed_;

		// 行列計算をしないで回転
		world_.rotation_ = FaceToDirection(velocity_);
	} else {
		isHoming_ = false;
	}
}
