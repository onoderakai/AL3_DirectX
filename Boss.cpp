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
	uint32_t hpTexture = TextureManager::Load("boss_hp.png");
	for (uint32_t i = 0; i < kMaxHp_; i++) {
		hpSprite_[i] =
		    Sprite::Create(hpTexture, {48 + (56.0f * i), 16}, {1, 1, 1, 1}, {0.5f, 0.5f});
		hpSprite_[i]->SetSize(hpSprite_[i]->GetSize() * 2.0f);
	}
}

Boss::~Boss() {
	for (BossBullet* bullet : bullets_) {
		delete bullet;
	}
	delete homingBulletModel_;
	for (uint32_t i = 0; i < kMaxHp_; i++) {
		delete hpSprite_[i];
	}
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

	switch (state_) {
	case Boss::State::LATERAL_MOVE:
		LateralMoveUpdate();
		break;
	case Boss::State::VERTICAL_MOVE:
		VerticalMoveUpdate();
		break;
	case Boss::State::EASE:
		EaseMoveUpdate();
		break;
	default:
		break;
	}
#ifdef _DEBUG
	ImGui::Begin("hp");
	ImGui::Text("%d", hp_);
	ImGui::End();
#endif // _DEBUG
	
	world_.UpdateMatrix();
}

void Boss::Draw(const ViewProjection& view) {
	if (isDead_) {
		return;
	}

	model_->Draw(world_, view);
	for (BossBullet* bullet : bullets_) {
		bullet->Draw(view);
	}
}

void Boss::DrawUI() {
	for (int i = 0; i < hp_; i++) {
		hpSprite_[i]->Draw();
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
		isDead_ = true;
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

void Boss::LateralMoveUpdate() {}

void Boss::VerticalMoveUpdate() {}

void Boss::EaseMoveUpdate() {}
