#include "Boss.h"
#include "CollisionConfig.h"
#include "ImGuiManager.h"
#include "ParticleSystem.h"
#include "Player.h"
#include <cassert>
#include "MathUtility.h"

Boss::Boss() {
	world_.scale_.x *= 3.0f;
	world_.scale_.y *= 3.0f;
	world_.scale_.z *= 3.0f;
	homingBulletModel_ = Model::Create();
}

Boss::~Boss() {
	for (BossBullet* bullet : bullets_) {
		delete bullet;
	}
	delete homingBulletModel_;
}

void Boss::Initialize(Model* model) {
	assert(model);
	model_ = model;
	world_.Initialize();

	bullets_.remove_if([](BossBullet* bullet) {
		delete bullet;
		return true;
	});

	isDead_ = false;
	hp_ = kMaxHp_;
	Attack();
	// 衝突フィルタリングを設定
	// このクラスの属性を設定
	SetCollisonAttribute(kCollisionAttributeBoss);
	// このクラスの衝突しない属性を設定
	SetCollisonMask(kCollisionAttributeBoss ^ kCollisionAttributeEnemy ^ GetCollisionMask());
}

void Boss::Update() {
	// デスフラグがtrueの弾を削除する
	bullets_.remove_if([](BossBullet* bullet) {
		if (bullet->GetIsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	world_.rotation_ = FaceToDirection(player_->GetWorldPosition() - GetWorldPosition());

	for (BossBullet* bullet : bullets_) {
		bullet->Update();
	}
#ifdef _DEBUG
	ImGui::Begin("hp");
	ImGui::Text("%d", hp_);
	ImGui::End();
#endif // _DEBUG
	//
	world_.UpdateMatrix();
}

void Boss::Draw(const ViewProjection& view) {
	model_->Draw(world_, view);
	for (BossBullet* bullet : bullets_) {
		bullet->Draw(view);
	}
}

Vector3 Boss::GetWorldPosition() {
	Vector3 worldPos = {};
	worldPos = {world_.matWorld_.m[3][0], world_.matWorld_.m[3][1], world_.matWorld_.m[3][2]};
	return worldPos;
}

void Boss::OnCollision() {
	hp_--;
	if (hp_ <= 0) {
		Particle::Parameter para = {};
		para.type_ = Particle::Type::SPHERE;
		para.world_.translation_ = GetWorldPosition();
		para.world_.scale_ = Vector3{2.0f, 2.0f, 2.0f};
		particleSystem_->Generate(para, 10);
	}
}

void Boss::Attack() {
	BossBullet* newBullet = new BossBullet();
	newBullet->Initialize(homingBulletModel_);
	newBullet->SetPlayer(player_);
	bullets_.push_back(newBullet);
}
