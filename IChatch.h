#pragma once
#include "Sprite.h"
#include "TextureManager.h"
#include "Easing.h"

class IChatch {
public:
	enum class IChatchCount {
		START,
		COUNT_1,
		COUNT_2,
		COUNT_3
	};

	IChatch();

	~IChatch();

	void Initialize();

	void Update();

	void Draw();

	bool GetIsEnd() { return isEnd_; }

private:
	Easing* easing_ = nullptr;
	Sprite* iChatch_[4];
	Sprite* backGround_ = nullptr;
	Vector2 startSize_[4] = {};

	bool isEnd_ = false;
	IChatchCount count_ = IChatchCount::COUNT_3;
	
	Vector2 start_ = {};
	Vector2 end_ = {};
	bool isEase_ = true;
	const uint32_t kMaxStopTime_ = 20;
	const uint32_t kMaxEasingTime_ = 40;
	int32_t stopTime_ = kMaxStopTime_;
};
