#include "Enemy.h"
#include "MathUtility.h"
#include <cassert>

void Enemy::Initialeze(Model* model, const Vector3& pos) {
	(assert(model));
	model_ = model;
	textureHandle_ = TextureManager::Load("/cube/cube.jpg");

	world_.Initialize();
	world_.translation_ = pos;
	velocity_ = {-0.3f, 0.3f, 0.0f};
}

void Enemy::Update() {
	switch (state_) {
	case Enemy::AttackState::APPROACH:
		ApproachUpdate();
		break;
	case Enemy::AttackState::LEAVE:
		LeaveUpdate();
		break;
	default:
		break;
	}

	world_.UpdateMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection) {
	model_->Draw(world_, viewProjection, textureHandle_);
}

void Enemy::ApproachUpdate() {
	world_.translation_.z -= 0.3f;
	// velocity_ = TransformNormal(velocity_, world_.matWorld_);
	if (world_.translation_.z < 0.0f) {
		state_ = AttackState::LEAVE;
	}
}

void Enemy::LeaveUpdate() {
	world_.translation_ += velocity_;
}
