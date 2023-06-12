#include "Enemy.h"
#include "MathUtility.h"
#include <cassert>
#include "Player.h"

void Enemy::Initialeze(Model* model, const Vector3& pos) {
	(assert(model));
	model_ = model;
	textureHandle_ = TextureManager::Load("/cube/cube.jpg");

	world_.Initialize();
	world_.translation_ = pos;
	velocity_ = {-0.3f, 0.3f, 0.0f};
}

void Enemy::Update() {
	//デスフラグがtrueの弾を削除する
	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->GetIsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

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

	//攻撃処理
	if (++attackCount >= attackInterval) {
		attackCount = 0;
		Attack();
	}

	//弾の更新処理を呼ぶ
	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}

	world_.UpdateMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection) {
	model_->Draw(world_, viewProjection, textureHandle_);

	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Enemy::OnCollision() {

}

Vector3 Enemy::GetWorldPosition() {
	Vector3 worldPos = {};
	worldPos = {world_.matWorld_.m[3][0], world_.matWorld_.m[3][1], world_.matWorld_.m[3][2]};
	return worldPos;
}

void Enemy::ApproachUpdate() {
	world_.translation_.z -= 0.3f;
	if (world_.translation_.z < 0.0f) {
		state_ = AttackState::LEAVE;
	}
}

void Enemy::LeaveUpdate() { world_.translation_ += velocity_; }

void Enemy::Attack() {
	// 弾のベクトルをプレイヤーの向きにする
	assert(player_);
	Vector3 bulletVelocity = player_->GetWorldPosition() - world_.translation_;
	bulletVelocity = Normalize(bulletVelocity);
	//弾の移動速度
	float bulletSpeed = 1.0f;
	bulletVelocity.x *= bulletSpeed;
	bulletVelocity.y *= bulletSpeed;
	bulletVelocity.z *= bulletSpeed;
	//bulletVelocity = TransformNormal(bulletVelocity, world_.matWorld_);
	
	//弾を生成
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(world_.translation_, bulletVelocity, model_);
	// 生成した弾をリストに追加
	bullets_.push_back(newBullet);
}
