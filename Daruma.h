#pragma once
#include "DarumaType.h"
#include "Easing.h"
#include "Input.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Shake.h"
#include <cstdint>

class Daruma {
public:
	Daruma();

	~Daruma();

	void Initialize(Model* model, const Vector3& pos, const DarumaType& type);

	void Update();

	void InputUpdate();

	void Draw(const ViewProjection& view);

	bool GetIsBreak() { return isBreak_; }

	Vector3 GetWorldPosition();

	int32_t GetPenaltyTime() { return penaltyTime_; }

	void SetPenaltyTime(uint32_t penaltyTime) { penaltyTime_ = penaltyTime; }

	void SetIsBreak(bool isBreak) { isBreak_ = isBreak; }

	void SetMovePos(const Vector3& movePos) { movePos_ = movePos; }

	void SetEaseStartPos(const Vector3& easeStartPos) {
		easeStartPos_ = easeStartPos;
		isEasing_ = true;
	}

private:
	Input* input_ = nullptr;
	// ジョイスティックを使う
	XINPUT_STATE joyState_ = {};
	XINPUT_STATE preJoyState_ = {};

	Easing* easing_ = nullptr;
	Shake* shake_ = nullptr;

	DarumaType type_ = DarumaType::RED;
	Model* model_ = nullptr;
	WorldTransform world_;

	uint32_t arrayNum_ = 0;
	bool isBreak_ = false;
	bool isEasing_ = false;
	Vector3 easeStartPos_ = {};
	Vector3 movePos_ = {};

	const uint32_t kMaxPenaltyTime_ = 60;
	int32_t penaltyTime_ = 0;

	void UpdateGreen(XINPUT_STATE& joyState, XINPUT_STATE& preJoyState);

	void UpdateRed(XINPUT_STATE& joyState, XINPUT_STATE& preJoyState);

	void UpdateBlue(XINPUT_STATE& joyState, XINPUT_STATE& preJoyState);

	void UpdateYellow(XINPUT_STATE& joyState, XINPUT_STATE& preJoyState);
};
