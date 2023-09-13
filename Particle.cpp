#include "Particle.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include "ViewProjection.h"
#include <cassert>

Particle::Particle() { textureHandle_ = TextureManager::Load("white1x1.png"); }

void Particle::Initialize(const Parameter& parameter, const Vector3& velocity, Model* model) {
	assert(model);
	parameter_ = parameter;
	model_ = model;
	parameter_.type_ = parameter.type_;

	parameter_.world_.Initialize();
	parameter_.world_.translation_ = parameter.world_.translation_;
	parameter_.world_.rotation_ = parameter.world_.rotation_;
	velocity_ = velocity;

	switch (parameter_.type_) {
	case Particle::Type::CIRCLE:
		parameter_.world_.scale_ = parameter.world_.scale_;
		sizeChange.x = float(parameter_.world_.scale_.x / parameter_.deathTimer_);
		sizeChange.y = float(parameter_.world_.scale_.y / parameter_.deathTimer_);
		sizeChange.z = float(parameter_.world_.scale_.z / parameter_.deathTimer_);
		break;
	case Particle::Type::SPHERE:
		parameter_.world_.scale_ = parameter.world_.scale_;
		sizeChange.x = float(parameter_.world_.scale_.x / parameter_.deathTimer_);
		sizeChange.y = float(parameter_.world_.scale_.y / parameter_.deathTimer_);
		sizeChange.z = float(parameter_.world_.scale_.z / parameter_.deathTimer_);
		break;
	case Particle::Type::SCALE_CHANGE:
		targetScale_ = {10.0f, 1.0f, 1.0f};
		parameter_.world_.scale_ = Vector3{0.0f, 0.0f, 0.0f};
		sizeChange.x = float(2.0f / parameter_.deathTimer_) * targetScale_.x;
		sizeChange.y = float(2.0f / parameter_.deathTimer_) * targetScale_.y;
		sizeChange.z = float(2.0f / parameter_.deathTimer_) * targetScale_.z;
		break;
	case Type::RIGHT_PARTICLE:
		parameter_.world_.scale_ = parameter.world_.scale_;
		sizeChange.x = float(parameter_.world_.scale_.x / parameter_.deathTimer_);
		sizeChange.y = float(parameter_.world_.scale_.y / parameter_.deathTimer_);
		sizeChange.z = float(parameter_.world_.scale_.z / parameter_.deathTimer_);
		if (velocity_.x < 0) {
			velocity_.x *= -1.0f;
		}
		break;
	case Type::GATHER_PARTICLE:
		parameter_.world_.scale_ = parameter.world_.scale_;
		sizeChange.x = float(parameter_.world_.scale_.x / parameter_.deathTimer_);
		sizeChange.y = float(parameter_.world_.scale_.y / parameter_.deathTimer_);
		sizeChange.z = float(parameter_.world_.scale_.z / parameter_.deathTimer_);
		break;
	default:
		break;
	}
}

void Particle::Initialize(
    const Parameter& parameter, const Vector3& velocity, Model* model, uint32_t textureHandle) {
	Initialize(parameter, velocity, model);
	textureHandle_ = textureHandle;
}

void Particle::Update() {
	// デスタイマーの計測
	if (--parameter_.deathTimer_ <= 0) {
		isDead_ = true;
	}

	switch (parameter_.type_) {
	case Particle::Type::CIRCLE:
		TypeCircleUpdate();
		break;
	case Particle::Type::SPHERE:
		TypeSphereUpdate();
		break;
	case Particle::Type::SCALE_CHANGE:
		TypeScaleChangeUpdate();
		break;
	case Particle::Type::RIGHT_PARTICLE:
		TypeSphereUpdate();
		break;
	case Particle::Type::GATHER_PARTICLE:
		TypeGatherUpdate();
		break;
	default:
		break;
	}
	parameter_.world_.UpdateMatrix();
}

void Particle::Draw(const ViewProjection& view) {
	model_->Draw(parameter_.world_, view, textureHandle_);
}

Vector3 Particle::GetWorldPosition() {
	Vector3 worldPos = {};
	worldPos = {
	    parameter_.world_.matWorld_.m[3][0], parameter_.world_.matWorld_.m[3][1],
	    parameter_.world_.matWorld_.m[3][2]};
	return worldPos;
}

void Particle::TypeCircleUpdate() {
	parameter_.world_.translation_ += velocity_;
	parameter_.world_.scale_ -= sizeChange;
}

void Particle::TypeSphereUpdate() {
	parameter_.world_.translation_ += velocity_;
	parameter_.world_.scale_ -= sizeChange;
}

void Particle::TypeScaleChangeUpdate() {
	parameter_.world_.scale_ += sizeChange;
	if (parameter_.world_.scale_.x >= targetScale_.x &&
	    parameter_.world_.scale_.y >= targetScale_.y &&
	    parameter_.world_.scale_.z >= targetScale_.z) {
		sizeChange.y *= -1.0f;
		sizeChange.z *= -1.0f;
	}
}

void Particle::TypeGatherUpdate() {
	parameter_.world_.scale_ -= sizeChange;
	Vector3 toGather = Vector3{-60.0f, 25.0f, 0.0f} - GetWorldPosition();
	toGather = Normalize(toGather);
	velocity_ = Normalize(velocity_);
	velocity_ = Lerp(velocity_, toGather, lerpAcceleration_) * parameter_.speed_;
	lerpAcceleration_ += 0.005f * (1.0f + lerpAcceleration_);
	parameter_.speed_ += 0.1f;
	parameter_.world_.translation_ += velocity_;
}
