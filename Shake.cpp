#include "Shake.h"
#include "MathUtility.h"

Shake::Shake() { easing_ = new Easing(); }

Shake::~Shake() { delete easing_; }

Vector3 Shake::Shaking(const WorldTransform& world) {
	if (!isShake_) {
		return world.translation_;
	}

	Vector3 returnPos = world.translation_;

	//  揺らす
	if (shakeFrame_ > 0) {
		shakeFrame_--;

		if (shakeRange_.x > 0.0f && shakeRange_.y > 0.0f) {
			shake_.x = float(rand() % 1000 - 500) / 500.0f * shakeRange_.x;
			shake_.y = float(rand() % 1000 - 500) / 500.0f * shakeRange_.y;
		}

		Vector3 forward = {shake_.x, shake_.y, 0.0f};
		// ワールド行列の反映
		forward = TransformNormal(forward, world.matWorld_);
		// シェイク
		returnPos = tmpPos_ + forward;

		// 揺れ幅を小さくしていく
		if (shakeRange_.x > 0.0f) {
			shakeRange_.x -= shakeRangeSubtract_.x;
			if (shakeRange_.x <= 0) {
				shakeRange_.x = 0.0f;
			}
		}
		if (shakeRange_.y > 0.0f) {
			shakeRange_.y -= shakeRangeSubtract_.y;
			if (shakeRange_.y <= 0) {
				shakeRange_.y = 0.0f;
			}
		}
	} else {
		isShake_ = false;
		returnPos = tmpPos_;
	}

	return returnPos;
}

Vector3 Shake::EasingShaking(const WorldTransform& world) {
	if (!isShake_) {
		return world.translation_;
	}

	Vector3 returnPos = world.translation_;

	//  揺らす
	if (shakeFrame_ > 0) {
		shakeFrame_--;

		if (!isEase_ && shakeRange_.x > 0.0f && shakeRange_.y > 0.0f) {
			isEase_ = true;
			isReturnEase_ = false;
			shake_.x = float(rand() % 1000 - 500) / 500.0f * shakeRange_.x;
			shake_.y = float(rand() % 1000 - 500) / 500.0f * shakeRange_.y;
		}
		Vector3 forward = {shake_.x, shake_.y, 0.0f};
		// ワールド行列の反映
		forward = TransformNormal(forward, world.matWorld_);

		if (isReturnEase_) {
			// シェイク
			Vector3 easingStartPos = tmpPos_ + forward;
			Vector3 easingEndPos = tmpPos_;
			returnPos = easing_->ConstantEase(returnPos, easingStartPos, easingEndPos, 5, isEase_);
		} else {
			// シェイク
			Vector3 easingStartPos = tmpPos_;
			Vector3 easingEndPos = tmpPos_ + forward;
			returnPos = easing_->ConstantEase(returnPos, easingStartPos, easingEndPos, 5, isEase_);
		}
		if (!isEase_ && !isReturnEase_) {
			isReturnEase_ = true;
			isEase_ = true;
		}

		// 揺れ幅を小さくしていく
		if (shakeRange_.x > 0.0f) {
			shakeRange_.x -= shakeRangeSubtract_.x;
			if (shakeRange_.x <= 0) {
				shakeRange_.x = 0.0f;
			}
		}
		if (shakeRange_.y > 0.0f) {
			shakeRange_.y -= shakeRangeSubtract_.y;
			if (shakeRange_.y <= 0) {
				shakeRange_.y = 0.0f;
			}
		}
	} else {
		isShake_ = false;
		returnPos = tmpPos_;
	}

	return returnPos;
}

void Shake::SetShaking(
    const bool& isShake, const uint32_t& shakeFrame, const Vector2& shakeRange,
    const Vector3& startPos) {
	if (isShake_) {
		return;
	}

	if (shakeFrame != 0) {
		shakeRangeSubtract_.y = shakeRange.y / float(shakeFrame);
		shakeRangeSubtract_.x = shakeRange.x / float(shakeFrame);
	}

	isShake_ = isShake;
	isReturnEase_ = false;
	shakeFrame_ = shakeFrame;
	shakeRange_ = shakeRange;
	tmpPos_ = startPos;
}
