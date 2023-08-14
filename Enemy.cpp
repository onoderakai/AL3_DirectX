#include "Enemy.h"
#include "GameScene.h"
#include "MathUtility.h"
#include "Player.h"
#include <cassert>

void Enemy::Initialeze(Model* model, const Vector3& pos) {
	(assert(model));
	model_ = model;
	textureHandle_ = TextureManager::Load("/cube/cube.jpg");

	world_.Initialize();
	world_.translation_ = pos;
	velocity_ = {-0.3f, 0.3f, 0.0f};

	//AttackReset();
}

void Enemy::Update() {
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	// デスフラグがtrueのTimeCallを削除する
	timeCalls_.remove_if([](TimeCall* timeCall) {
		if (timeCall->IsFinished()) {
			delete timeCall;
			return true;
		}
		return false;
	});

	for (TimeCall* timeCall : timeCalls_) {
		timeCall->Update();
	}

	switch (state_) {
	case Enemy::AttackState::APPROACH:
		ApproachUpdate();
		break;
	case Enemy::AttackState::LEAVE:
		LeaveUpdate();
		timeCalls_.clear();
		break;
	default:
		break;
	}

	world_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(world_, viewProjection, textureHandle_);
}

void Enemy::OnCollision() {
	Particle::Parameter para = {};
	para.world_.translation_ = GetWorldPosition();
	particleSystem_->Generate(para, 10);
	isDead_ = true;
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
	// 弾の移動速度
	float bulletSpeed = 1.0f;
	bulletVelocity.x *= bulletSpeed;
	bulletVelocity.y *= bulletSpeed;
	bulletVelocity.z *= bulletSpeed;
	// bulletVelocity = TransformNormal(bulletVelocity, world_.matWorld_);

	// 弾を生成
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(world_.translation_, bulletVelocity, model_);
	newBullet->SetPlayer(player_);
	// 生成した弾をリストに追加
	gameScene_->AddEnemyBullet(newBullet);
}

void Enemy::AttackReset() {
	// 弾を発射する
	Attack();

	// 発射タイマーをセットする
	timeCalls_.push_back(new TimeCall(bind(&Enemy::AttackReset, this), attackInterval));
}
