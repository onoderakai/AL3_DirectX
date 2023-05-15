#include "PlayerBullet.h"
#include <cassert>

void PlayerBullet::Initialeze(Model* model, const Vector3& pos, const Vector3& velocity) {
	(assert(model));
	model_ = model;

	textureHandle_ = TextureManager::Load("white1x1.png");

	world_.Initialize();
	world_.translation_ = pos;

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
