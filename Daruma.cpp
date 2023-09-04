#include "Daruma.h"

Daruma::Daruma() { input_ = Input::GetInstance(); }

Daruma::~Daruma() {}

void Daruma::Update(const uint32_t& arrayNum) {
	arrayNum_ = arrayNum;
	if (arrayNum_ != 0) {
		return;
	}

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
}

void Daruma::Draw(const ViewProjection& view) {
	if (isBreak_) {
		return;
	}
	model_->Draw(world_, view);
}

void Daruma::UpdateGreen() {
	if (input_->TriggerKey(DIK_G)) {
		isBreak_ = true;
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
