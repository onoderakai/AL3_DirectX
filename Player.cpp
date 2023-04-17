#include "Player.h"
#include <cassert>

void Player::Initialeze(Model* model, uint32_t textureHandle) { 
	(assert(model));
	(assert(textureHandle));
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
}

void Player::Uppdate() { 
	//行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

void Player::Draw(ViewProjection& viewProjection) {

	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
