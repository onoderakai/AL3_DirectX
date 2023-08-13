#include "Particle.h"
#include "ViewProjection.h"
#include "TextureManager.h"
#include <cassert>

void Particle::Initialize(
    const Vector3& pos, const Vector3& velocity, Type particleType, uint32_t textureHandle, Model* model) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	type_ = particleType;
	world_.Initialize();
	world_.translation_ = pos;
	velocity_ = velocity;
}

void Particle::Initialize(Model* model) {
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("white1x1.png");
	velocity_.x = 1.0f;
}

void Particle::Update() {
	switch (type_) {
	case Particle::Type::CIRCLE:
		TypeCircleUpdate();
		break;
	case Particle::Type::Sphere:
		TypeSphereUpdate();
		break;
	default:
		break;
	}
	world_.UpdateMatrix();
}

void Particle::Draw(const ViewProjection& view) { model_->Draw(world_, view, textureHandle_); }

void Particle::TypeCircleUpdate() { world_.translation_ += velocity_; }

void Particle::TypeSphereUpdate() { world_.translation_ += velocity_; }
