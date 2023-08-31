#include "BossBullet.h"
#include "CollisionConfig.h"
#include "MathUtility.h"
#include "Player.h"
#include <cassert>

void BossBullet::Initialize(Model* model, AttackType attackType, const Vector3& pos) {
	assert(model);
	model_ = model;

	world_.Initialize();
	attackType_ = attackType;
	world_.translation_ = pos;
	world_.scale_ = world_.scale_ * 2.0f;

	// 衝突フィルタリングを設定
	// このクラスの属性を設定
	SetCollisonAttribute(kCollisionAttributeBoss);
	// このクラスの衝突しない属性を設定
	SetCollisonMask(
	    kCollisionAttributeBoss ^ kCollisionAttributeEnemy ^ GetCollisionMask() ^
	    kCollisionAttributeBullet);
}

void BossBullet::Initialize(
    Model* model, AttackType attackType, const Vector3& pos, const Vector3& velocity) {
	Initialize(model, attackType, pos);
	velocity_ = velocity;
}

void BossBullet::Update() {
	hp_--;
	if (hp_ <= 0) {
		isDead_ = true;
	}
	switch (attackType_) {
	case BossBullet::AttackType::NORMAL:
		AttackNormalUpdate();
		break;
	case BossBullet::AttackType::HOMING:
		AttackHomingUpdate();
		break;
	default:
		break;
	}

	world_.translation_ += velocity_;

	world_.UpdateMatrix();
}

void BossBullet::Draw(const ViewProjection& view) { model_->Draw(world_, view); }

void BossBullet::OnCollision() {
	hp_--;
	isDead_ = true;
	if (hp_ <= 0) {
	}
}

Vector3 BossBullet::GetWorldPosition() {
	Vector3 worldPos = {};
	worldPos = {world_.matWorld_.m[3][0], world_.matWorld_.m[3][1], world_.matWorld_.m[3][2]};
	return worldPos;
}

void BossBullet::AttackNormalUpdate() {}

void BossBullet::AttackHomingUpdate() {
	if (!isHoming_) {
		return;
	}

	float homingLength = 45.0f;
	if (Length(player_->GetWorldPosition() - GetWorldPosition()) >= homingLength) {
		// ホーミング
		Vector3 toPlayer = player_->GetWorldPosition() - GetWorldPosition();
		// ベクトルを正規化する
		toPlayer = Normalize(toPlayer);
		velocity_ = Normalize(velocity_);
		// 球面線形補間により、今の速度と自キャラへのベクトルを内挿し、新たな速度とする
		speed_ *= 1.01f;
		velocity_ = Lerp(velocity_, toPlayer, 0.2f) * speed_;

		// 行列計算をしないで回転
		world_.rotation_ = FaceToDirection(velocity_);
	} else {
		isHoming_ = false;
	}
}
