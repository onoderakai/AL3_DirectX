#include "Player.h"
#include "MathUtility.h"
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

	//デスフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->GetIsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	//移動処理
	Vector3 move = {};
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
	// 移動
	world_.translation_ += move;


	// 旋回処理
	Vector3 rotate = {};
	const float matRotSpeed = 0.02f;
	//入力
	if (input_->PushKey(DIK_UP)) {
		rotate.x = -matRotSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		rotate.x = matRotSpeed;
	}
	if (input_->PushKey(DIK_RIGHT)) {
		rotate.y = matRotSpeed;
	} else if (input_->PushKey(DIK_LEFT)) {
		rotate.y = -matRotSpeed;
	}
	//旋回
	world_.rotation_ += rotate;

	//攻撃処理
	Attack();
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
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

	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

Vector3 Player::GetWorldPosition() {
	Vector3 worldPos = {};
	worldPos.x = world_.translation_.x;
	worldPos.y = world_.translation_.y;
	worldPos.z = world_.translation_.z;
	return worldPos;
}

void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE)) {
		// 自キャラの座標
		Vector3 pos = world_.translation_;

		PlayerBullet* newBullet = new PlayerBullet();
		Vector3 bulletVelocity = {0.0f, 0.0f, 1.0f};
		bulletVelocity = TransformNormal(bulletVelocity, world_.matWorld_);
		newBullet->Initialeze(model_, pos, bulletVelocity);

		bullets_.push_back(newBullet);
	}
}
