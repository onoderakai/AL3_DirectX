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
	textureHandle_ = TextureManager::Load("/cube/cube.jpg");

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
		//isDead_ = true;
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
	case Enemy::Type::NORMAL:
		NormalUpdate();
		break;
	case Enemy::Type::TO_PLAYER:
		ToPlayerUpdate();
		break;
	case Enemy::Type::HOMING:
		HomingUpdate();
		break;
	default:
		break;
	}

	//timeCalls_.clear();

	world_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(world_, viewProjection, textureHandle_);
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

void Enemy::HomingAttack() {
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
	switch (type_) {
	case Enemy::Type::NORMAL:
		break;
	case Enemy::Type::TO_PLAYER:
		break;
	case Enemy::Type::HOMING:
		// 弾を発射する
		HomingAttack();
		break;
	default:
		break;
	}

	// 発射タイマーをセットする
	timeCalls_.push_back(new TimeCall(bind(&Enemy::AttackReset, this), attackInterval));
}

void Enemy::NormalUpdate() {
	if (world_.translation_.z >= 0.0f) {
		world_.translation_.z -= 0.3f;
	}
}

void Enemy::ToPlayerUpdate() {
	if (world_.translation_.z >= 0.0f) {
		world_.translation_.z -= 0.3f;
	}
}

void Enemy::HomingUpdate() {
	if (world_.translation_.z >= 0.0f) {
		world_.translation_.z -= 0.3f;
	}
}
