#include "Enemy.h"
#include "CollisionConfig.h"
#include "GameScene.h"
#include "ImGuiManager.h"
#include "MathUtility.h"
#include "Player.h"
#include "TextureManager.h"
#include <cassert>

Enemy::Enemy() { particleTextureHandle_ = TextureManager::Load("red1x1.png"); }

void Enemy::Initialize(Model* model, const Vector3& pos) {
	(assert(model));
	model_ = model;

	world_.Initialize();
	world_.translation_ = pos;
	velocity_ = {-0.3f, 0.3f, 0.0f};

	AttackReset();

	// 衝突フィルタリングを設定
	// このクラスの属性を設定
	SetCollisonAttribute(kCollisionAttributeEnemy);
	// このクラスの衝突しない属性を設定
	SetCollisonMask(kCollisionAttributeEnemy ^ GetCollisionMask());
}

void Enemy::Initialize(Type type, Model* model, const Vector3& pos) {
	type_ = type;
	Initialize(model, pos);
}

void Enemy::Update() {
	if (--deathTimer_ <= 0) {
		// isDead_ = true;
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

	// 種類別の行動パターン
	switch (type_) {
	case Type::NORMAL:
		isAttack = true;
		NormalUpdate();
		break;
	case Type::TO_PLAYER:
		isAttack = true;
		ToPlayerUpdate();
		break;
	case Type::HOMING:
		HomingUpdate();
		break;
	default:
		break;
	}

	// timeCalls_.clear();

	world_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(world_, viewProjection);
}

void Enemy::OnCollision() {
	Particle::Parameter para = {};
	para.type_ = Particle::Type::SCALE_CHANGE;
	para.world_.translation_ = GetWorldPosition();
	particleSystem_->Generate(para, 10, particleTextureHandle_);
	para.world_.scale_ = Vector3{2.0f, 2.0f, 2.0f};
	para.type_ = Particle::Type::SPHERE;
	particleSystem_->Generate(para, 10);
	isDead_ = true;
}

Vector3 Enemy::GetWorldPosition() {
	Vector3 worldPos = {};
	worldPos = {world_.matWorld_.m[3][0], world_.matWorld_.m[3][1], world_.matWorld_.m[3][2]};
	return worldPos;
}

void Enemy::AttackReset() {
	if (isAttack) {
		switch (type_) {
		case Type::NORMAL:
			NormalAttack();
			break;
		case Type::TO_PLAYER:
			ToPlayerAttack();
			break;
		case Type::HOMING:
			HomingAttack();
			break;
		default:
			break;
		}
	}

	// 発射タイマーをセットする
	timeCalls_.push_back(new TimeCall(bind(&Enemy::AttackReset, this), attackInterval));
}

void Enemy::NormalUpdate() {
	if (world_.translation_.z >= 0.0f) {
		world_.translation_.z -= 0.3f;
	} else {
		world_.translation_.x += velocity_.x;
		world_.translation_.y += velocity_.y;
		if (world_.translation_.x >= 70.0f) {
			velocity_.x *= -1.0f;
		}
		if (world_.translation_.x <= -70.0f) {
			velocity_.x *= -1.0f;
		}
		if (world_.translation_.y >= 40.0f) {
			velocity_.y *= -1.0f;
		}
		if (world_.translation_.y <= -40.0f) {
			velocity_.y *= -1.0f;
		}
	}
}

void Enemy::NormalAttack() {
	Vector3 bulletVelocity = {0.0f, 0.0f, -3.0f};
	// 弾を生成
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(type_, world_.translation_, bulletVelocity, model_);
	newBullet->SetPlayer(player_);
	// 生成した弾をリストに追加
	gameScene_->AddEnemyBullet(newBullet);
}

void Enemy::ToPlayerUpdate() {
	if (world_.translation_.z >= 0.0f) {
		world_.translation_.z -= 0.3f;
	}
}

void Enemy::ToPlayerAttack() {
	// 弾のベクトルをプレイヤーの向きにする
	assert(player_);
	Vector3 bulletVelocity = player_->GetWorldPosition() - world_.translation_;
	bulletVelocity = Normalize(bulletVelocity);
	// 弾の移動速度
	float bulletSpeed = 1.0f;
	bulletVelocity.x *= bulletSpeed;
	bulletVelocity.y *= bulletSpeed;
	bulletVelocity.z *= bulletSpeed;

	// 弾を生成
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(type_, world_.translation_, bulletVelocity, model_);
	newBullet->SetPlayer(player_);
	// 生成した弾をリストに追加
	gameScene_->AddEnemyBullet(newBullet);
}

void Enemy::HomingUpdate() {
	if (world_.translation_.z > 0.0f) {
		world_.translation_.z -= 2.0f;
		if (world_.translation_.z <= 0.0f) {
			world_.translation_.z = 0.0f;
			isAttack = true;
		}
	}
}

void Enemy::HomingAttack() {
	// 弾のベクトルをプレイヤーの向きにする
	assert(player_);
	Vector3 bulletVelocity = player_->GetWorldPosition() - world_.translation_;
	bulletVelocity = Normalize(bulletVelocity);
	// 弾の移動速度
	float bulletSpeed = 0.5f;
	bulletVelocity.x *= bulletSpeed;
	bulletVelocity.y *= bulletSpeed;
	bulletVelocity.z *= bulletSpeed;

	// 弾を生成
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(type_, world_.translation_, bulletVelocity, model_);
	newBullet->SetPlayer(player_);
	// 生成した弾をリストに追加
	gameScene_->AddEnemyBullet(newBullet);
}