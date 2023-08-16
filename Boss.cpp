#include "Boss.h"
#include "CollisionConfig.h"
#include <cassert>

Boss::~Boss() {}

void Boss::Initialize() {
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

void Boss::Draw() {}
