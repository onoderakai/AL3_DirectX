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

	void Move();

	void Draw(const ViewProjection& view);

	bool GetIsBreak() { return isBreak_; }

	Vector3 GetWorldPosition();

	void SetIsBreak(bool isBreak) { isBreak_ = isBreak; }

	void SetMovePos(const Vector3& movePos) { movePos_ = movePos; }

	void SetEaseStartPos(const Vector3& easeStartPos) {
		easeStartPos_ = easeStartPos;
		isEasing_ = true;
	}

private:
	Input* input_ = nullptr;
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

	void UpdateGreen();

	void UpdateRed();

	void UpdateBlue();

	void UpdateYellow();
};
