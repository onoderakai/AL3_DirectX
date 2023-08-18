#include "Boss.h"
#include "CollisionConfig.h"
#include <cassert>

Boss::~Boss() {}

void Boss::Initialize(Model* model) {
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("target.png");
	world_.Initialize();

	// 衝突フィルタリングを設定
	// このクラスの属性を設定
	SetCollisonAttribute(kCollisionAttributeBoss);
	// このクラスの衝突しない属性を設定
	SetCollisonMask(kCollisionAttributeBoss ^ kCollisionAttributeEnemy ^ GetCollisionMask());
}

void Boss::Update() {
	//
	world_.UpdateMatrix();
}

void Boss::Draw(ViewProjection view) { model_->Draw(world_, view, textureHandle_); }
