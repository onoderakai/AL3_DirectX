#pragma once
#include "Sprite.h"
#include "TextureManager.h"
#include "Easing.h"
#include "SoundManager.h"

class Finish {
public:
	Finish();
	~Finish();

	void Initialize();

	void Update();

	void Draw();

	bool GetIsFinish() { return isFinish_; }

	bool GetIsResult() { return isResult_; }

	void SetFinishTime(uint32_t finishTime) { maxFinishCount_ = finishTime; }

	void SetIsTimeSystem(bool isTimeSystem) { isTimeSystem_ = isTimeSystem; }

private:
	Easing* easing_ = nullptr;
	Sprite* finishSprite_ = nullptr;
	SoundManager* sound_ = nullptr;

	uint32_t maxFinishCount_ = 600;
	uint32_t finishCount_ = 0;
	const uint32_t kMaxFinishStopCount_ = 65;
	int32_t finishStopCount_ = kMaxFinishStopCount_;

	bool isTimeSystem_ = true;
	bool isFinish_ = false;
	bool isFinishSound_ = false;

	bool isEase_ = false;
	bool isResult_ = false;
	Vector2 start = {};
	Vector2 end = {};
};
