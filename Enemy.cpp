#include "Enemy.h"
#include <cassert>

void Enemy::Initialeze(Model* model, const Vector3& pos) {
	(assert(model));
	model_ = model;
	textureHandle_ = TextureManager::Load("/cube/cube.jpg");

	world_.Initialize();
	world_.translation_ = pos;
	velocity_ = {0.0f, 0.0f, -0.3f};
}

void Enemy::Update() {
	world_.translation_ += velocity_;

	world_.UpdateMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection) {
	model_->Draw(world_, viewProjection, textureHandle_);
}
