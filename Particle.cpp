#include "Particle.h"
#include "ViewProjection.h"
#include "TextureManager.h"
#include <cassert>

void Particle::Initialize(Parameter parameter, uint32_t textureHandle, Model* model) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	parameter_.type_ = parameter.type_;
	parameter_.world_.Initialize();
	parameter_.world_.translation_ = parameter.world_.translation_;
	parameter_.velocity_ = parameter.velocity_;
}

void Particle::Initialize(Parameter parameter, Model* model) {
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("white1x1.png");
	parameter_.type_ = parameter.type_;
	parameter_.world_.Initialize();
	parameter_.world_.translation_ = parameter.world_.translation_;
	parameter_.velocity_ = parameter.velocity_;
}

void Particle::Update() {
	//デスタイマーの計測
	if (--parameter_.deathTimer_ <= 0) {
		isDead_ = true;
	}

	switch (parameter_.type_) {
	case Particle::Type::CIRCLE:
		TypeCircleUpdate();
		break;
	case Particle::Type::Sphere:
		TypeSphereUpdate();
		break;
	default:
		break;
	}
	parameter_.world_.UpdateMatrix();
}

void Particle::Draw(const ViewProjection& view) {
	model_->Draw(parameter_.world_, view, textureHandle_);
}

void Particle::TypeCircleUpdate() { parameter_.world_.translation_ += parameter_.velocity_; }

void Particle::TypeSphereUpdate() { parameter_.world_.translation_ += parameter_.velocity_; }
