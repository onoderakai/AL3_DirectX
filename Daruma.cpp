#include "Daruma.h"
#include "ImGuiManager.h"

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
	// ジョイスティックを使う
	XINPUT_STATE joyState = {};
	XINPUT_STATE preJoyState = {};
	// 接続状態を確認
	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}
	if (!Input::GetInstance()->GetJoystickStatePrevious(0, preJoyState)) {
		return;
	}

	ImGui::Begin("a");
	ImGui::Text("%d", penaltyTime_);
	ImGui::End();

	// シェイク中とペナルティ中はペナルティタイムを減らし続ける
	if (shake_->GetIsShake() || penaltyTime_ > 0) {
		penaltyTime_--;
	}

	switch (type_) {
	case DarumaType::GREEN:
		UpdateGreen(joyState, preJoyState);
		break;
	case DarumaType::RED:
		UpdateRed(joyState, preJoyState);
		break;
	case DarumaType::BLUE:
		UpdateBlue(joyState, preJoyState);
		break;
	case DarumaType::YELLOW:
		UpdateYellow(joyState, preJoyState);
		break;
	default:
		break;
	}
	world_.UpdateMatrix();
}

void Daruma::Move() {
	world_.translation_ =
	    easing_->EaseOutElastic(world_.translation_, easeStartPos_, movePos_, 10, isEasing_);
	if (!isEasing_) {
		world_.translation_ = shake_->Shaking(world_);
	}

	world_.UpdateMatrix();
}

void Daruma::Draw(const ViewProjection& view) {
	if (isBreak_) {
		world_.translation_.x += -8.0f;
	}
	model_->Draw(world_, view);
}

Vector3 Daruma::GetWorldPosition() {
	Vector3 worldPos = {};
	worldPos = {world_.matWorld_.m[3][0], world_.matWorld_.m[3][1], world_.matWorld_.m[3][2]};
	return worldPos;
}

void Daruma::UpdateGreen(XINPUT_STATE& joyState, XINPUT_STATE& preJoyState) {
	if (!shake_->GetIsShake() && (input_->TriggerKey(DIK_G) ||
	    (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A &&
	                                  (preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) == 0))) {
		isBreak_ = true;
	} else if (
	    input_->TriggerKey(DIK_R) || input_->TriggerKey(DIK_B) || input_->TriggerKey(DIK_Y) ||
	    (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B &&
	     (preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) == 0) ||
	    (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_X &&
	     (preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_X) == 0) ||
	    (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_Y &&
	     (preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_Y) == 0)) {
		penaltyTime_ = kMaxPenaltyTime_;
		shake_->SetShaking(true, 30, Vector2{1.0f, 1.0f}, GetWorldPosition());
	}
}

void Daruma::UpdateRed(XINPUT_STATE& joyState, XINPUT_STATE& preJoyState) {
	if (!shake_->GetIsShake() && (input_->TriggerKey(DIK_R) ||
	    (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B &&
	                                  (preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) == 0))) {
		isBreak_ = true;
	} else if (
	    input_->TriggerKey(DIK_G) || input_->TriggerKey(DIK_B) || input_->TriggerKey(DIK_Y) ||
	    (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A &&
	     (preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) == 0) ||
	    (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_X &&
	     (preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_X) == 0) ||
	    (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_Y &&
	     (preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_Y) == 0)) {
		penaltyTime_ = kMaxPenaltyTime_;
		shake_->SetShaking(true, 30, Vector2{1.0f, 1.0f}, GetWorldPosition());
	}
}

void Daruma::UpdateBlue(XINPUT_STATE& joyState, XINPUT_STATE& preJoyState) {
	if (!shake_->GetIsShake() && (input_->TriggerKey(DIK_B) ||
	    (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_X &&
	                                  (preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_X) == 0))) {
		isBreak_ = true;
	} else if (
	    input_->TriggerKey(DIK_R) || input_->TriggerKey(DIK_G) || input_->TriggerKey(DIK_Y) ||
	    (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B &&
	     (preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) == 0) ||
	    (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A &&
	     (preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) == 0) ||
	    (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_Y &&
	     (preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_Y) == 0)) {
		penaltyTime_ = kMaxPenaltyTime_;
		shake_->SetShaking(true, 30, Vector2{1.0f, 1.0f}, GetWorldPosition());
	}
}

void Daruma::UpdateYellow(XINPUT_STATE& joyState, XINPUT_STATE& preJoyState) {
	if (!shake_->GetIsShake() && (input_->TriggerKey(DIK_Y) ||
	    (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_Y &&
	                                  (preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_Y) == 0))) {
		isBreak_ = true;
	} else if (
	    input_->TriggerKey(DIK_R) || input_->TriggerKey(DIK_B) || input_->TriggerKey(DIK_G) ||
	    (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B &&
	     (preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) == 0) ||
	    (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_X &&
	     (preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_X) == 0) ||
	    (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A &&
	     (preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) == 0)) {
		penaltyTime_ = kMaxPenaltyTime_;
		shake_->SetShaking(true, 30, Vector2{1.0f, 1.0f}, GetWorldPosition());
	}
}
