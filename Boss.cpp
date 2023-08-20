#include "Boss.h"
#include "CollisionConfig.h"
#include "ImGuiManager.h"
#include "ParticleSystem.h"
#include <cassert>

Boss::Boss() { textureHandle_ = TextureManager::Load("target.png"); }

Boss::~Boss() {}

void Boss::Initialize(Model* model) {
	assert(model);
	model_ = model;
	world_.Initialize();

	isDead_ = false;
	hp_ = kMaxHp_;

	// 衝突フィルタリングを設定
	// このクラスの属性を設定
	SetCollisonAttribute(kCollisionAttributeBoss);
	// このクラスの衝突しない属性を設定
	SetCollisonMask(kCollisionAttributeBoss ^ kCollisionAttributeEnemy ^ GetCollisionMask());
}

void Boss::Update() {
#ifdef _DEBUG
	ImGui::Begin("hp");
	ImGui::Text("%d", hp_);
	ImGui::End();
#endif // _DEBUG
	//
	world_.UpdateMatrix();
}

void Boss::Draw(ViewProjection view) { model_->Draw(world_, view, textureHandle_); }

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
