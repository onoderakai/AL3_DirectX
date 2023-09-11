#include "Finish.h"

Finish::Finish() {
	easing_ = new Easing();

	uint32_t finishTex = TextureManager::Load("finish.png");
	finishSprite_ = Sprite::Create(finishTex, Vector2{0.0f, -720.0f});
	start = finishSprite_->GetPosition();
	end = Vector2{0.0f, 100.0f};
}

Finish::~Finish() {
	delete finishSprite_;
	delete easing_;
}

void Finish::Initialize() {
	isFinish_ = false;
	finishCount_ = 0;
	easing_->Initialize();
	isEase_ = false;
	isResult_ = false;
	finishStopCount_ = kMaxFinishStopCount_;
}

void Finish::Update() {
	finishCount_++;
	if (finishCount_ >= kMaxFinishCount_ && !isFinish_) {
		isFinish_ = true;
		isEase_ = true;
	}
	finishSprite_->SetPosition(
	    easing_->EaseOutElastic(finishSprite_->GetPosition(), start, end, 30, isEase_));
	if (!isEase_ && isFinish_) {
		finishStopCount_--;
		if (finishStopCount_ <= 0) {
			isResult_ = true;
		}
	}
}

void Finish::Draw() {
	if (!isFinish_ || isResult_) {
		return;
	}

	finishSprite_->Draw();
}
