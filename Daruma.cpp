#include "Daruma.h"
#include "ImGuiManager.h"

Daruma::Daruma() {
	input_ = Input::GetInstance();
	easing_ = new Easing();
	shake_ = new Shake();
	sound_ = SoundManager::GetInstance();
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
	arrayNum_ = 0;
	isBreak_ = false;
	isEasing_ = false;
	easeStartPos_ = {};
	penaltyTime_ = 0;
	easing_->Initialize();
	shake_->Initialize();

	world_.UpdateMatrix();
}

void Daruma::Update() {
	if (isBreak_) {
		world_.translation_.x += -8.0f;
		world_.UpdateMatrix();
		return;
	}
	switch (type_) {
	case DarumaType::GREEN:
		world_.rotation_.y += 0.005f;
		break;
	case DarumaType::RED:
		world_.rotation_.y += 0.005f;
		break;
	case DarumaType::BLUE:
		world_.rotation_.y += 0.005f;
		break;
	case DarumaType::YELLOW:
		world_.rotation_.y += 0.005f;
		break;
	case DarumaType::TOP:
		world_.rotation_.y += 0.005f;
		break;
	case DarumaType::BREAK:
		break;
	default:
		break;
	}
	world_.translation_ =
	    easing_->EaseOutElastic(world_.translation_, easeStartPos_, movePos_, 30, isEasing_);
	if (!isEasing_) {
		world_.translation_ = shake_->Shaking(world_);
	}
	world_.UpdateMatrix();
}

void Daruma::InputUpdate() {
	// シェイク中とペナルティ中はペナルティタイムを減らし続ける
	if (!isEasing_ && (shake_->GetIsShake() || penaltyTime_ > 0)) {
		penaltyTime_--;
	}

	ImGui::Begin("a");
	ImGui::Text("%d", penaltyTime_);
	ImGui::End();

	switch (type_) {
	case DarumaType::GREEN:
		UpdateGreen(joyState_, preJoyState_);
		break;
	case DarumaType::RED:
		UpdateRed(joyState_, preJoyState_);
		break;
	case DarumaType::BLUE:
		UpdateBlue(joyState_, preJoyState_);
		break;
	case DarumaType::YELLOW:
		UpdateYellow(joyState_, preJoyState_);
		break;
	default:
		break;
	}

	// 接続状態を確認
	if (!Input::GetInstance()->GetJoystickState(0, joyState_)) {
		return;
	}
	if (!Input::GetInstance()->GetJoystickStatePrevious(0, preJoyState_)) {
		return;
	}
}

void Daruma::Draw(const ViewProjection& view) {
	model_->Draw(world_, view);
}

Vector3 Daruma::GetWorldPosition() {
	Vector3 worldPos = {};
	worldPos = {world_.matWorld_.m[3][0], world_.matWorld_.m[3][1], world_.matWorld_.m[3][2]};
	return worldPos;
}

void Daruma::UpdateGreen(XINPUT_STATE& joyState, XINPUT_STATE& preJoyState) {
	if (!shake_->GetIsShake() &&
	    (input_->TriggerKey(DIK_G) || (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A &&
	                                   (preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) == 0))) {
		isBreak_ = true;
		sound_->OnPlaySound(SoundManager::Sound::SE_DARUMA_BREAK);

		Particle::Parameter para = {};
		para.world_.translation_ = GetWorldPosition();
		para.type_ = Particle::Type::RIGHT_PARTICLE;
		particleSystem_->Generate(para, 10);
	} else if (
	    input_->TriggerKey(DIK_R) || input_->TriggerKey(DIK_B) || input_->TriggerKey(DIK_Y) ||
	    (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B &&
	     (preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) == 0) ||
	    (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_X &&
	     (preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_X) == 0) ||
	    (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_Y &&
	     (preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_Y) == 0)) {
		penaltyTime_ = kMaxPenaltyTime_;
		sound_->OnPlaySound(SoundManager::Sound::SE_MISS_TOUCH);
		shake_->SetShaking(true, 30, Vector2{1.0f, 1.0f}, movePos_);
	}
}

void Daruma::UpdateRed(XINPUT_STATE& joyState, XINPUT_STATE& preJoyState) {
	if (!shake_->GetIsShake() &&
	    (input_->TriggerKey(DIK_R) || (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B &&
	                                   (preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) == 0))) {
		isBreak_ = true;
		sound_->OnPlaySound(SoundManager::Sound::SE_DARUMA_BREAK);

		Particle::Parameter para = {};
		para.world_.translation_ = GetWorldPosition();
		para.type_ = Particle::Type::RIGHT_PARTICLE;
		particleSystem_->Generate(para, 10);
	} else if (
	    input_->TriggerKey(DIK_G) || input_->TriggerKey(DIK_B) || input_->TriggerKey(DIK_Y) ||
	    (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A &&
	     (preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) == 0) ||
	    (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_X &&
	     (preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_X) == 0) ||
	    (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_Y &&
	     (preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_Y) == 0)) {
		penaltyTime_ = kMaxPenaltyTime_;
		sound_->OnPlaySound(SoundManager::Sound::SE_MISS_TOUCH);
		shake_->SetShaking(true, 30, Vector2{1.0f, 1.0f}, movePos_);
	}
}

void Daruma::UpdateBlue(XINPUT_STATE& joyState, XINPUT_STATE& preJoyState) {
	if (!shake_->GetIsShake() &&
	    (input_->TriggerKey(DIK_B) || (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_X &&
	                                   (preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_X) == 0))) {
		isBreak_ = true;
		sound_->OnPlaySound(SoundManager::Sound::SE_DARUMA_BREAK);

		Particle::Parameter para = {};
		para.world_.translation_ = GetWorldPosition();
		para.type_ = Particle::Type::RIGHT_PARTICLE;
		particleSystem_->Generate(para, 10);

	} else if (
	    input_->TriggerKey(DIK_R) || input_->TriggerKey(DIK_G) || input_->TriggerKey(DIK_Y) ||
	    (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B &&
	     (preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) == 0) ||
	    (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A &&
	     (preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) == 0) ||
	    (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_Y &&
	     (preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_Y) == 0)) {
		penaltyTime_ = kMaxPenaltyTime_;
		sound_->OnPlaySound(SoundManager::Sound::SE_MISS_TOUCH);
		shake_->SetShaking(true, 30, Vector2{1.0f, 1.0f}, movePos_);
	}
}

void Daruma::UpdateYellow(XINPUT_STATE& joyState, XINPUT_STATE& preJoyState) {
	if (!shake_->GetIsShake() &&
	    (input_->TriggerKey(DIK_Y) || (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_Y &&
	                                   (preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_Y) == 0))) {
		isBreak_ = true;
		sound_->OnPlaySound(SoundManager::Sound::SE_DARUMA_BREAK);

		Particle::Parameter para = {};
		para.world_.translation_ = GetWorldPosition();
		para.type_ = Particle::Type::RIGHT_PARTICLE;
		particleSystem_->Generate(para, 10);
	} else if (
	    input_->TriggerKey(DIK_R) || input_->TriggerKey(DIK_B) || input_->TriggerKey(DIK_G) ||
	    (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B &&
	     (preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) == 0) ||
	    (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_X &&
	     (preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_X) == 0) ||
	    (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A &&
	     (preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) == 0)) {
		penaltyTime_ = kMaxPenaltyTime_;
		sound_->OnPlaySound(SoundManager::Sound::SE_MISS_TOUCH);
		shake_->SetShaking(true, 30, Vector2{1.0f, 1.0f}, movePos_);
	}
}
