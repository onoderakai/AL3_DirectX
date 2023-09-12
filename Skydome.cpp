#include "Skydome.h"
#include <cassert>

void Skydome::Initialize(Model* model) {
	assert(model);
	model_ = model;
	world_.Initialize();
	world_.translation_ = {0.0f, 0.0f, 0.0f};
}

void Skydome::Update() {
	world_.rotation_.y += 0.003f;
	world_.UpdateMatrix();
}

void Skydome::Draw(ViewProjection viewProjection) {
	model_->Draw(world_, viewProjection);
}
