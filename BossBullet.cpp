#include "BossBullet.h"
#include "CollisionConfig.h"

void BossBullet::Initialize() {

	// 衝突フィルタリングを設定
	// このクラスの属性を設定
	SetCollisonAttribute(kCollisionAttributeBoss);
	// このクラスの衝突しない属性を設定
	SetCollisonMask(kCollisionAttributeBoss ^ kCollisionAttributeEnemy ^ GetCollisionMask());
}

void BossBullet::Update() {}

void BossBullet::Draw() {}
