#include "EnemyBullet.h"
#include <cassert>

void EnemyBullet::Initialize(const Vector3& pos, const Vector3& velocity, Model* model) {
	assert(model);
	velocity_ = velocity;
	world_.Initialize();
	world_.translation_ = pos;

	//弾の大きさ
	/*world_.scale_.x = 0.5f;
	world_.scale_.y = 0.5f;
	world_.scale_.z = 3.0f;*/

	//world_.rotation_.y = std::atan2f(velocity_.z, velocity_.x);
	//Matrix4x4 rot = MakeRotationYMatrix(-std::atan2f(velocity_.z, velocity_.x));
	////Vector3 velocityZ = Multiply(velocity_, rot);
	//Vector3 velocityZ = {};
	//velocityZ.x = velocity_.x * rot.m[0][0] + velocity_.y * rot.m[0][1] + velocity_.z * rot.m[0][2];
	//velocityZ.y = velocity_.x * rot.m[1][0] + velocity_.y * rot.m[1][1] + velocity_.z * rot.m[1][2];
	//velocityZ.z = velocity_.x * rot.m[2][0] + velocity_.y * rot.m[2][1] + velocity_.z * rot.m[2][2];

	//world_.rotation_.x = std::atan2f(velocity_.z, velocity_.y);

	model_ = model;
	textureHandle_ = TextureManager::Load("white1x1.png");
}

void EnemyBullet::Update() {
	if (--deathTimer_ <= 0) {
		isDead_ = true;
		return;
	}

	world_.translation_ += velocity_;

	world_.UpdateMatrix();
}

void EnemyBullet::Draw(ViewProjection& viewProjection) {
	model_->Draw(world_, viewProjection, textureHandle_);
}

void EnemyBullet::OnCollision() {
	isDead_ = true;
}
