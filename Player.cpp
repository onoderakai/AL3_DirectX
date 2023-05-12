#include "Player.h"
#include "ImGuiManager.h"
#include <cassert>

void Player::Initialeze(Model* model, uint32_t textureHandle) {
	(assert(model));
	(assert(textureHandle));
	model_ = model;
	textureHandle_ = textureHandle;
	world_.Initialize();
	input_ = Input::GetInstance();
}

void Player::Update() {

	Vector3 move = {0.0f, 0.0f, 0.0f};
	const float kCharacterSpeed = 0.2f;

	// 入力
	if (input_->PushKey(DIK_A)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_D)) {
		move.x += kCharacterSpeed;
	}
	if (input_->PushKey(DIK_W)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_S)) {
		move.y -= kCharacterSpeed;
	}

	Attack();
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	// 移動
	world_.translation_.x += move.x;
	world_.translation_.y += move.y;
	world_.translation_.z += move.z;

	// 旋回
	const float matRotSpeed = 0.2f;
	if (input_->PushKey(DIK_UP)) {
		world_.rotation_.x -= matRotSpeed;
	}
	else if (input_->PushKey(DIK_DOWN)) {
		world_.rotation_.x += matRotSpeed;
	}
	if (input_->PushKey(DIK_RIGHT)) {
		world_.rotation_.y -= matRotSpeed;
	}
	else if (input_->PushKey(DIK_LEFT)) {
		world_.rotation_.y += matRotSpeed;
	}

	// 当たり判定
	float moveLimitX = 34.0f;
	float moveLimitY = 18.0f;

	world_.translation_.x = max(world_.translation_.x, -moveLimitX);
	world_.translation_.x = min(world_.translation_.x, +moveLimitX);
	world_.translation_.y = max(world_.translation_.y, -moveLimitY);
	world_.translation_.y = min(world_.translation_.y, +moveLimitY);

	world_.UpdateMatrix();

#ifdef DEBUG
	// ImGui
	ImGui::Begin("PlayerPos");
	ImGui::Text(
	    "x:%f:y:%f:z:%f", worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z);
	ImGui::SliderFloat3("", &worldTransform_.translation_.x, 0.0f, 10.0f);
	ImGui::End();

#endif // DEBUG
}

void Player::Draw(ViewProjection& viewProjection) {

	model_->Draw(world_, viewProjection, textureHandle_);

	for (PlayerBullet* bullet : bullets_)
	{
		bullet->Draw(viewProjection);
	}
}

void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE)) {
		//自キャラの座標
		Vector3 pos = world_.translation_;

		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialeze(model_, pos);

		bullets_.push_back(newBullet);
	}

}
