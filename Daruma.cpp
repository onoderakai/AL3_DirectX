#include "Daruma.h"

Daruma::Daruma() {
	input_ = Input::GetInstance();
	easing_ = new Easing();
}

Daruma::~Daruma() {
	delete easing_;
	delete shake_;
}

void Daruma::Initialize(Model* model, const Vector3& pos, const DarumaType& type) {
	model_ = model;
	world_.Initialize();
	world_.translation_ = pos;
	movePos_ = pos;
	type_ = type;
	world_.scale_ = {5.0f, 5.0f, 5.0f};
	shake_ = new Shake();

	world_.UpdateMatrix();
}

void Daruma::Update() {
	switch (type_) {
	case DarumaType::GREEN:
		UpdateGreen();
		break;
	case DarumaType::RED:
		UpdateRed();
		break;
	case DarumaType::BLUE:
		UpdateBlue();
		break;
	case DarumaType::YELLOW:
		UpdateYellow();
		break;
	default:
		break;
	}
	world_.UpdateMatrix();
}

void Daruma::Move() {
	world_.translation_ =
	    easing_->EaseOutElastic(world_.translation_, easeStartPos_, movePos_, 10, isEasing_);
	world_.translation_ = shake_->Shaking(world_);

	world_.UpdateMatrix();
}

void Daruma::Draw(const ViewProjection& view) {
	if (isBreak_) {
		return;
	}
	model_->Draw(world_, view);
}

Vector3 Daruma::GetWorldPosition() {
	Vector3 worldPos = {};
	worldPos = {world_.matWorld_.m[3][0], world_.matWorld_.m[3][1], world_.matWorld_.m[3][2]};
	return worldPos;
}

void Daruma::UpdateGreen() {
	if (input_->TriggerKey(DIK_G)) {
		isBreak_ = true;
	}
	else if (input_->TriggerKey(DIK_R) ||
		input_->TriggerKey(DIK_B) ||
		input_->TriggerKey(DIK_Y)) {
		shake_->SetShaking(true, 30, Vector2{30.0f, 30.0f}, GetWorldPosition());
	}
}

void Daruma::UpdateRed() {
	if (input_->TriggerKey(DIK_R)) {
		isBreak_ = true;
	}
}

void Daruma::UpdateBlue() {
	if (input_->TriggerKey(DIK_B)) {
		isBreak_ = true;
	}
}

void Daruma::UpdateYellow() {
	if (input_->TriggerKey(DIK_Y)) {
		isBreak_ = true;
	}
}
