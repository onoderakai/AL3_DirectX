#include "IChatch.h"

IChatch::IChatch() {
	sound_ = SoundManager::GetInstance();

	uint32_t iChatch3Tex = TextureManager::Load("num/3.png");
	uint32_t iChatch2Tex = TextureManager::Load("num/2.png");
	uint32_t iChatch1Tex = TextureManager::Load("num/1.png");
	uint32_t iChatch0Tex = TextureManager::Load("ichatch_start.png");
	uint32_t bgTex = TextureManager::Load("white1x1.png");
	easing_ = new Easing();

	backGround_ = Sprite::Create(bgTex, Vector2{0.0f, 0.0f}, Vector4{0.0f, 0.0f, 0.0f, 0.9f});
	backGround_->SetSize(Vector2{1280.0f, 720.0f});
	iChatch_[3] = Sprite::Create(
	    iChatch3Tex, Vector2{0.0f, -720.0f}, Vector4{1.0f, 1.0f, 1.0f, 1.0f}, Vector2{0.5f, 0.5f});
	iChatch_[2] = Sprite::Create(
	    iChatch2Tex, Vector2{0.0f, -720.0f}, Vector4{1.0f, 1.0f, 1.0f, 1.0f}, Vector2{0.5f, 0.5f});
	iChatch_[1] = Sprite::Create(
	    iChatch1Tex, Vector2{0.0f, -720.0f}, Vector4{1.0f, 1.0f, 1.0f, 1.0f}, Vector2{0.5f, 0.5f});
	iChatch_[0] = Sprite::Create(
	    iChatch0Tex, Vector2{0.0f, -720.0f}, Vector4{1.0f, 1.0f, 1.0f, 1.0f}, Vector2{0.5f, 0.5f});
	iChatch_[0]->SetSize(Vector2{});

	// 大きさを保存
	for (uint32_t i = 0; i < 4; i++) {
		startSize_[i] = iChatch_[i]->GetSize();
	}
}

IChatch::~IChatch() {
	for (uint32_t i = 0; i < 4; i++) {
		delete iChatch_[i];
	}
	delete backGround_;
	delete easing_;
}

void IChatch::Initialize() {
	for (uint32_t i = 0; i < 4; i++) {
		iChatch_[i]->SetPosition(Vector2{0.0f, -720.0f});
		iChatch_[i]->SetColor(Vector4{1.0f, 1.0f, 1.0f, 1.0f});
		iChatch_[i]->SetSize(startSize_[i]);
	}
	iChatch_[0]->SetPosition(Vector2{640.0f, 360.0f});

	easing_->Initialize();
	isEase_ = true;
	isEnd_ = false;
	count_ = IChatchCount::COUNT_3;
	start_ = Vector2{640.0f, -720.0f};
	end_ = Vector2{640.0f, 360.0f};
}

void IChatch::Update() {
	if (isEnd_) {
		return;
	}

	Vector4 color = {};
	Vector2 size = {};
	switch (count_) {
	case IChatch::IChatchCount::START:
		if (stopTime_ > 0) {
			stopTime_--;
			if (stopTime_ <= 0) {
				sound_->OnPlaySound(SoundManager::Sound::SE_ICHATCH_START);
			}
		} else {
			iChatch_[0]->SetSize(easing_->EaseOutElastic(
			    iChatch_[0]->GetSize(), Vector2{}, Vector2{768.0f, 256.0f}, kMaxEasingTime_,
			    isEase_));
		}

		// ひとつ前のカウントの画像を徐々に透明にしつつ、大きくする
		color = iChatch_[1]->GetColor();
		size = iChatch_[1]->GetSize();
		color.w -= 1.0f / kMaxStopTime_;
		size += Vector2{3.0f, 3.0f};
		if (color.w <= 0.0f) {
			color.w = 0.0f;
		}
		iChatch_[1]->SetColor(color);
		iChatch_[1]->SetSize(size);

		if (!isEase_ && stopTime_ <= 0) {
			stopTime_ = kMaxStopTime_;
			isEnd_ = true;
		}
		break;
	case IChatch::IChatchCount::COUNT_1:
		iChatch_[1]->SetPosition(easing_->EaseOutSine(
		    iChatch_[1]->GetPosition(), start_, end_, kMaxEasingTime_, isEase_));

		// ひとつ前のカウントの画像を徐々に透明にしつつ、大きくする
		color = iChatch_[2]->GetColor();
		size = iChatch_[2]->GetSize();
		color.w -= 1.0f / kMaxStopTime_;
		size += Vector2{3.0f, 3.0f};
		if (color.w <= 0.0f) {
			color.w = 0.0f;
		}
		iChatch_[2]->SetColor(color);
		iChatch_[2]->SetSize(size);

		if (!isEase_ && stopTime_ <= 0) {
			stopTime_ = kMaxEasingTime_ * 2 - kMaxStopTime_;
			isEase_ = true;
			sound_->OnPlaySound(SoundManager::Sound::SE_ICHATCH_COUNT);
			count_ = IChatchCount::START;
		} else if (!isEase_) {
			stopTime_--;
		}
		break;
	case IChatch::IChatchCount::COUNT_2:
		iChatch_[2]->SetPosition(easing_->EaseOutSine(
		    iChatch_[2]->GetPosition(), start_, end_, kMaxEasingTime_, isEase_));

		// ひとつ前のカウントの画像を徐々に透明にしつつ、大きくする
		color = iChatch_[3]->GetColor();
		size = iChatch_[3]->GetSize();
		color.w -= 1.0f / kMaxStopTime_;
		size += Vector2{3.0f, 3.0f};
		if (color.w <= 0.0f) {
			color.w = 0.0f;
		}
		iChatch_[3]->SetColor(color);
		iChatch_[3]->SetSize(size);

		if (!isEase_ && stopTime_ <= 0) {
			stopTime_ = kMaxStopTime_;
			isEase_ = true;
			sound_->OnPlaySound(SoundManager::Sound::SE_ICHATCH_COUNT);
			count_ = IChatchCount::COUNT_1;
		} else if (!isEase_) {
			stopTime_--;
		}
		break;
	case IChatch::IChatchCount::COUNT_3:
		iChatch_[3]->SetPosition(easing_->EaseOutSine(
		    iChatch_[3]->GetPosition(), start_, end_, kMaxEasingTime_, isEase_));
		if (!isEase_ && stopTime_ <= 0) {
			stopTime_ = kMaxStopTime_;
			isEase_ = true;
			sound_->OnPlaySound(SoundManager::Sound::SE_ICHATCH_COUNT);
			count_ = IChatchCount::COUNT_2;
		} else if (!isEase_) {
			stopTime_--;
		}
		break;
	default:
		break;
	}
}

void IChatch::Draw() {
	if (isEnd_) {
		return;
	}

	backGround_->Draw();
	for (uint32_t i = 0; i < 4; i++) {
		iChatch_[i]->Draw();
	}
}
