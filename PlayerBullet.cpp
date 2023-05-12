#include "PlayerBullet.h"
#include <cassert>

void PlayerBullet::Initialeze(Model* model, const Vector3& pos) {
	(assert(model));
	model_ = model;

	textureHandle_ = TextureManager::Load("white1x1.png");

	world_.Initialize();
	world_.translation_ = pos;
}

void PlayerBullet::Update() {
	world_.translation_.z += 1.0f;
	world_.UpdateMatrix();
}

void PlayerBullet::Draw(ViewProjection& viewProjection) {
	model_->Draw(world_, viewProjection, textureHandle_);
}
