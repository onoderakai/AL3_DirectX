#pragma once
#include "Vector3.h"
#include "Vector2.h"
#include "WorldTransform.h"
#include <stdint.h>
#include "Easing.h"

class Shake {
public:
	Shake();
	~Shake();

	void Initialize();

	Vector3 Shaking(
	    const WorldTransform& world);
	Vector3 EasingShaking(const WorldTransform& world);

	bool GetIsShake() { return isShake_; }

	void SetShaking(
	    const bool& isShake, const uint32_t& shakeFrame, const Vector2& shakeRange,
	    const Vector3& startPos);

private:
	Easing* easing_ = nullptr;
	Vector3 tmpPos_ = {};
	Vector2 shakeRange_ = {};
	Vector2 shake_ = {};
	uint32_t shakeFrame_ = 0;
	Vector2 shakeRangeSubtract_ = {};
	bool isShake_ = false;
	bool isEase_ = false;
	bool isReturnEase_ = false;
};
