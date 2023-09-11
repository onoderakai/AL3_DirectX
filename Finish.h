#pragma once
#include "Sprite.h"
#include "TextureManager.h"
#include "Easing.h"

class Finish {
public:
	Finish();
	~Finish();

	void Initialize();

	void Update();

	void Draw();

	bool GetIsFinish() { return isFinish_; }

	bool GetIsResult() { return isResult_; }

private:
	Easing* easing_ = nullptr;
	Sprite* finishSprite_ = nullptr;

	const uint32_t kMaxFinishCount_ = 600;
	uint32_t finishCount_ = 0;
	const uint32_t kMaxFinishStopCount_ = 60;
	int32_t finishStopCount_ = kMaxFinishStopCount_;
	bool isFinish_ = false;

	bool isEase_ = false;
	bool isResult_ = false;
	Vector2 start = {};
	Vector2 end = {};
};
