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
	//モデルの生成
	normalBulletModel_ = Model::CreateFromOBJ("BossBulletNormal", true);
	homingBulletModel_ = Model::CreateFromOBJ("BossBulletHoming", true);

	//HPスプライトの生成
	uint32_t hpTexture = TextureManager::Load("boss_hp.png");
	for (uint32_t i = 0; i < kMaxHp_; i++) {
		hpSprite_[i] =
		    Sprite::Create(hpTexture, {48 + (56.0f * i), 16}, {1, 1, 1, 1}, {0.5f, 0.5f});
		hpSprite_[i]->SetSize(hpSprite_[i]->GetSize() * 2.0f);
	}

	//イージングクラスの作成
	easing_ = new Easing();
}

Boss::~Boss() {
	for (BossBullet* bullet : bullets_) {
		delete bullet;
	}
	delete homingBulletModel_;
	for (uint32_t i = 0; i < kMaxHp_; i++) {
		delete hpSprite_[i];
	}
	delete easing_;
	delete normalBulletModel_;
}

void Boss::Initialize(Model* model) {
	assert(model);
	model_ = model;
	world_.Initialize();
	world_.translation_ = {};

	bullets_.remove_if([](BossBullet* bullet) {
		delete bullet;
		return true;
	});

	state_ = State::EASE;
	isEase_ = false;
	isDead_ = false;
	hp_ = kMaxHp_;
	moveFrame_ = 0;

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

	moveFrame_++;
	if (moveFrame_ >= kChangeStateFrame_) {
		moveFrame_ = 0;
		easing_->ResetTimeCount();
		isEase_ = false;
		if (state_ == State::NORMAL_MOVE) {
			state_ = State::EASE;
		} else {
			state_ = State::NORMAL_MOVE;
		}
	}
	switch (state_) {
	case Boss::State::NORMAL_MOVE:
		NormalMoveUpdate();
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

	for (BossBullet* bullet : bullets_) {
		bullet->Update();
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

void Boss::NormalMoveUpdate() {
	if (!isEase_) {
		isEase_ = true;
		NormalBulletAttack();

		BossBullet* newBullet = new BossBullet();
		Vector3 bulletVelocity = player_->GetWorldPosition() - GetWorldPosition();
		bulletVelocity.x += 5.0f;
		bulletVelocity = Normalize(bulletVelocity) * 2.0f;
		newBullet->Initialize(
		    normalBulletModel_, BossBullet::AttackType::NORMAL, GetWorldPosition(), bulletVelocity);
		newBullet->SetPlayer(player_);
		newBullet->SetRadius(3.0f);
		bullets_.push_back(newBullet);

		BossBullet* newBullet2 = new BossBullet();
		bulletVelocity.x -= 0.2f;
		bulletVelocity = Normalize(bulletVelocity) * 2.0f;
		newBullet2->Initialize(
		    normalBulletModel_, BossBullet::AttackType::NORMAL, GetWorldPosition(), bulletVelocity);
		newBullet2->SetPlayer(player_);
		newBullet2->SetRadius(3.0f);
		bullets_.push_back(newBullet2);

		start = GetWorldPosition();
		end.x = float(rand() % 140 - 70);
		end.y = float(rand() % 80 - 40);
		end.z = float(rand() % 60);
	}
	world_.translation_ = easing_->ConstantEase(world_.translation_, start, end, 100, isEase_);
}

void Boss::VerticalMoveUpdate() {}

void Boss::EaseMoveUpdate() {
	if (!isEase_) {
		isEase_ = true;
		HomingBulletAttack();
		start = GetWorldPosition();
		end.x = float(rand() % 140 - 70);
		end.y = float(rand() % 80 - 40);
		end.z = float(rand() % 60);
	}
	world_.translation_ = easing_->EaseOutSine(world_.translation_, start, end, 60, isEase_);
}

void Boss::NormalBulletAttack() {
	BossBullet* newBullet = new BossBullet();
	Vector3 bulletVelocity = player_->GetWorldPosition() - GetWorldPosition();
	bulletVelocity = Normalize(bulletVelocity) * 2.0f;
	newBullet->Initialize(
	    normalBulletModel_, BossBullet::AttackType::NORMAL, GetWorldPosition(), bulletVelocity);
	newBullet->SetPlayer(player_);
	newBullet->SetRadius(3.0f);
	bullets_.push_back(newBullet);
}

void Boss::HomingBulletAttack() {
	BossBullet* newBullet = new BossBullet();
	newBullet->Initialize(homingBulletModel_, BossBullet::AttackType::HOMING, GetWorldPosition());
	newBullet->SetPlayer(player_);
	bullets_.push_back(newBullet);
}
