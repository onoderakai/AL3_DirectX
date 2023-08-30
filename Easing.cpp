#include "Easing.h"
#include <cmath>

Vector3 Easing::ConstantEase(
    const Vector3& value, const Vector3& start, const Vector3& end, int maxFrame, bool& isEase) {
	if (!isEase) {
		timeCount = 0;
		return value;
	}

	if (timeCount < maxFrame) {
		timeCount++;
		if (timeCount >= maxFrame) {
			isEase = false;
			return end;
		}
		Vector3 dis = end - start;
		float t = float(timeCount) / float(maxFrame);
		float ease = 1.0f * t;
		return (dis * ease) + start;
	}
	timeCount = 0;
	return value;
}

Vector3 Easing::EaseInSine(
    const Vector3& value, const Vector3& start, const Vector3& end, int maxFrame, bool& isEase) {
	if (!isEase) {
		timeCount = 0;
		return value;
	}

	if (timeCount < maxFrame) {
		timeCount++;
		if (timeCount >= maxFrame) {
			isEase = false;
			return end;
		}
		Vector3 dis = end - start;
		float t = float(timeCount) / float(maxFrame);
		float ease = 1.0f - cosf((t * PIf) / 2.0f);
		return (dis * ease) + start;
	}
	timeCount = 0;
	return value;
}

Vector3 Easing::EaseOutSine(
    const Vector3& value, const Vector3& start, const Vector3& end, int maxFrame, bool& isEase) {
	if (!isEase) {
		timeCount = 0;
		return value;
	}

	if (timeCount < maxFrame) {
		timeCount++;
		if (timeCount >= maxFrame) {
			isEase = false;
			return end;
		}
		Vector3 dis = end - start;
		float t = float(timeCount) / float(maxFrame);
		float ease = sinf((t * PIf) / 2.0f);
		return (dis * ease) + start;
	}
	timeCount = 0;
	return value;
}

Vector2 Easing::EaseOutSine(
    const Vector2& value, const Vector2& start, const Vector2& end, int maxFrame, bool& isEase) {
	if (!isEase) {
		timeCount = 0;
		return value;
	}

	if (timeCount < maxFrame) {
		timeCount++;
		if (timeCount >= maxFrame) {
			isEase = false;
			return end;
		}
		Vector2 dis = end - start;
		float t = float(timeCount) / float(maxFrame);
		float ease = sinf((t * PIf) / 2.0f);
		return (dis * ease) + start;
	}
	timeCount = 0;
	return value;
}

Vector3 Easing::EaseOutElastic(
    const Vector3& value, const Vector3& start, const Vector3& end, int maxFrame, bool& isEase) {
	if (!isEase) {
		timeCount = 0;
		return value;
	}
	const float c4 = (2.0f * PIf) / 3.0f;
	if (timeCount < maxFrame) {
		timeCount++;
		if (timeCount >= maxFrame) {
			isEase = false;
			return end;
		}
		Vector3 dis = end - start;
		float t = float(timeCount) / float(maxFrame);
		float ease = powf(2.0f, -10.0f * t) * sinf((t * 10 - 0.75f) * c4) + 1.0f;
		return (dis * ease) + start;
	}
	timeCount = 0;
	return value;
}
