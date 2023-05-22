#include "EnemyBullet.h"
#include <cassert>

void EnemyBullet::Initialize(const Vector3& pos, const Vector3& velocity, Model* model) {
	assert(model);
	velocity_ = velocity;
	world_.Initialize();
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
