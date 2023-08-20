#include "Particle.h"
#include "TextureManager.h"
#include "ViewProjection.h"
#include "ImGuiManager.h"
#include <cassert>

void Particle::Initialize(
    Parameter parameter, const Vector3& velocity, Model* model) {
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("white1x1.png");
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
		parameter_.world_.scale_ = Vector3{0.0f, 0.0f, 0.0f};
		sizeChange.x = 0.5f;
		sizeChange.y = float(2.0f / parameter_.deathTimer_);
		sizeChange.z = float(2.0f / parameter_.deathTimer_);
		break;
	default:
		break;
	}
	
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
	default:
		break;
	}
	ImGui::Begin("a");
	ImGui::Text(
	    "%f,%f,%f", parameter_.world_.rotation_.x, parameter_.world_.rotation_.y,
	    parameter_.world_.rotation_.z);
	ImGui::End();
	parameter_.world_.UpdateMatrix();
}

void Particle::Draw(const ViewProjection& view) {
	model_->Draw(parameter_.world_, view, textureHandle_);
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
	Vector3 targetScale = {10.0f, 1.0f, 1.0f};
	if (parameter_.world_.scale_.x >= targetScale.x &&
	    parameter_.world_.scale_.y >= targetScale.y &&
	    parameter_.world_.scale_.z >= targetScale.z) {
		sizeChange.y *= -1.0f;
		sizeChange.z *= -1.0f;
	}
}
