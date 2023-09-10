#include "Easing.h"
#include <cmath>

void Easing::Initialize() { timeCount_ = 0; }

Vector3 Easing::ConstantEase(
    const Vector3& value, const Vector3& start, const Vector3& end, const uint32_t& maxFrame,
    bool& isEase) {
	if (!isEase) {
		timeCount_ = 0;
		return value;
	}

	if (timeCount_ < maxFrame) {
		timeCount_++;
		if (timeCount_ >= maxFrame) {
			isEase = false;
			return end;
		}
		Vector3 dis = end - start;
		float t = float(timeCount_) / float(maxFrame);
		float ease = 1.0f * t;
		return (dis * ease) + start;
	}
	timeCount_ = 0;
	return value;
}

Vector3 Easing::EaseInSine(
    const Vector3& value, const Vector3& start, const Vector3& end, const uint32_t& maxFrame,
    bool& isEase) {
	if (!isEase) {
		timeCount_ = 0;
		return value;
	}

	if (timeCount_ < maxFrame) {
		timeCount_++;
		if (timeCount_ >= maxFrame) {
			isEase = false;
			return end;
		}
		Vector3 dis = end - start;
		float t = float(timeCount_) / float(maxFrame);
		float ease = 1.0f - cosf((t * PIf) / 2.0f);
		return (dis * ease) + start;
	}
	timeCount_ = 0;
	return value;
}

Vector3 Easing::EaseOutSine(
    const Vector3& value, const Vector3& start, const Vector3& end, const uint32_t& maxFrame,
    bool& isEase) {
	if (!isEase) {
		timeCount_ = 0;
		return value;
	}

	if (timeCount_ < maxFrame) {
		timeCount_++;
		if (timeCount_ >= maxFrame) {
			isEase = false;
			return end;
		}
		Vector3 dis = end - start;
		float t = float(timeCount_) / float(maxFrame);
		float ease = sinf((t * PIf) / 2.0f);
		return (dis * ease) + start;
	}
	timeCount_ = 0;
	return value;
}

Vector2 Easing::EaseOutSine(
    const Vector2& value, const Vector2& start, const Vector2& end, const uint32_t& maxFrame,
    bool& isEase) {
	if (!isEase) {
		timeCount_ = 0;
		return value;
	}

	if (timeCount_ < maxFrame) {
		timeCount_++;
		if (timeCount_ >= maxFrame) {
			isEase = false;
			return end;
		}
		Vector2 dis = end - start;
		float t = float(timeCount_) / float(maxFrame);
		float ease = sinf((t * PIf) / 2.0f);
		return (dis * ease) + start;
	}
	timeCount_ = 0;
	return value;
}

Vector3 Easing::EaseOutElastic(
    const Vector3& value, const Vector3& start, const Vector3& end, const uint32_t& maxFrame,
    bool& isEase) {
	if (!isEase) {
		timeCount_ = 0;
		return value;
	}
	const float c4 = (2.0f * PIf) / 3.0f;
	if (timeCount_ < maxFrame) {
		timeCount_++;
		if (timeCount_ >= maxFrame) {
			isEase = false;
			return end;
		}
		Vector3 dis = end - start;
		float t = float(timeCount_) / float(maxFrame);
		float ease = powf(2.0f, -10.0f * t) * sinf((t * 10 - 0.75f) * c4) + 1.0f;
		return (dis * ease) + start;
	}
	timeCount_ = 0;
	return value;
}

Vector2 Easing::EaseOutElastic(
    const Vector2& value, const Vector2& start, const Vector2& end, const uint32_t& maxFrame,
    bool& isEase) {
	if (!isEase) {
		timeCount_ = 0;
		return value;
	}
	const float c4 = (2.0f * PIf) / 3.0f;
	if (timeCount_ < maxFrame) {
		timeCount_++;
		if (timeCount_ >= maxFrame) {
			isEase = false;
			return end;
		}
		Vector2 dis = end - start;
		float t = float(timeCount_) / float(maxFrame);
		float ease = powf(2.0f, -10.0f * t) * sinf((t * 10 - 0.75f) * c4) + 1.0f;
		return (dis * ease) + start;
	}
	timeCount_ = 0;
	return value;
}
