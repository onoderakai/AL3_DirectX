#include "Score.h"
#include <stdio.h>

Score::Score() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 10; j++) {
			easing_[i][j] = new Easing();
			sprite2DNum_[i][j] = nullptr;
			sprite2DNumResult_[i][j] = nullptr;
		}
	};
	uint32_t textureNum[10] = {
	    TextureManager::Load("num/0.png"), TextureManager::Load("num/1.png"),
	    TextureManager::Load("num/2.png"), TextureManager::Load("num/3.png"),
	    TextureManager::Load("num/4.png"), TextureManager::Load("num/5.png"),
	    TextureManager::Load("num/6.png"), TextureManager::Load("num/7.png"),
	    TextureManager::Load("num/8.png"), TextureManager::Load("num/9.png"),
	};
	uint32_t textureNum2[10] = {
	    TextureManager::Load("num/0.png"), TextureManager::Load("num/1.png"),
	    TextureManager::Load("num/2.png"), TextureManager::Load("num/3.png"),
	    TextureManager::Load("num/4.png"), TextureManager::Load("num/5.png"),
	    TextureManager::Load("num/6.png"), TextureManager::Load("num/7.png"),
	    TextureManager::Load("num/8.png"), TextureManager::Load("num/9.png"),
	};

	float posX = 170.0f;
	float posXResult = 930.0f;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 10; j++) {
			sprite2DNum_[i][j] =
			    Sprite::Create(textureNum[j], {posX, 100}, {1, 1, 1, 1}, {(0.0f), (0.0f)});
			sprite2DTime_[i][j] =
			    Sprite::Create(textureNum[j], {posX, 25}, {1, 1, 1, 1}, {(0.0f), (0.0f)});
			sprite2DNumResult_[i][j] =
			    Sprite::Create(textureNum2[j], {posXResult, 400}, {1, 1, 1, 1}, {(0.0f), (0.0f)});
			sprite2DNumResult_[i][j]->SetSize(sprite2DNumResult_[i][j]->GetSize() * 2.0f);

			spriteSize_[i][j] = sprite2DNumResult_[i][j]->GetSize();
		}
		posX -= 50;
		posXResult -= 150;
	}
}

Score::~Score() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 10; j++) {
			delete easing_[i][j];
			delete sprite2DNum_[i][j];
			delete sprite2DTime_[i][j];
			delete sprite2DNumResult_[i][j];
		}
	}
}

void Score::Initialize() {
	tmpScore_ = 0;
	preScore_ = 0;
	addTmpScore_ = 0;
	isScoreDecision_ = false;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 10; j++) {
			easing_[i][j]->Initialize();
			isEase_[i][j] = true;
		}
	}
}

void Score::DrawTimeUI(int score) {
	if (score >= 10000) {
		score = 9999;
	}

	int eachNumber[4]{};
	int digit = 1000;
	for (int i = 0; i < 4; i++) {
		eachNumber[i] = score / digit;
		score -= eachNumber[i] * digit;
		digit /= 10;
	}
	sprite2DTime_[0][eachNumber[3]]->Draw();
	sprite2DTime_[1][eachNumber[2]]->Draw();
	sprite2DTime_[2][eachNumber[1]]->Draw();
	sprite2DTime_[3][eachNumber[0]]->Draw();
}

void Score::DrawScoreUI(int score) {
	if (score >= 10000) {
		score = 9999;
	}
	if (preScore_ != score) {
		addTmpScore_ = (score - tmpScore_) / 30;
	}
	preScore_ = score;
	if (tmpScore_ < score) {
		tmpScore_ += addTmpScore_;
		if (tmpScore_ > score) {
			tmpScore_ = score;
		}
	}
	int32_t scoreCount = tmpScore_;

	int eachNumber[4]{};
	int digit = 1000;
	for (int i = 0; i < 4; i++) {
		eachNumber[i] = scoreCount / digit;
		scoreCount -= eachNumber[i] * digit;
		digit /= 10;
	}
	sprite2DNum_[0][eachNumber[3]]->Draw();
	sprite2DNum_[1][eachNumber[2]]->Draw();
	sprite2DNum_[2][eachNumber[1]]->Draw();
	sprite2DNum_[3][eachNumber[0]]->Draw();
}

void Score::DrawScoreUIResult(int score, const bool& isDraw) {
	if (score >= 10000) {
		score = 9999;
	}
	if (score <= 0) {
		isScoreDecision_ = true;
	}

	if (preScore_ != score) {
		addTmpScore_ = (score - tmpScore_) / 30;
	}

	preScore_ = score;
	if (tmpScore_ < score) {
		tmpScore_ += addTmpScore_;
		if (tmpScore_ >= score) {
			tmpScore_ = score;
			isScoreDecision_ = true;
		}
	}

	int32_t scoreCount = tmpScore_;

	int eachNumber[4]{};
	int digit = 1000;
	for (int i = 0; i < 4; i++) {
		eachNumber[i] = scoreCount / digit;
		scoreCount -= eachNumber[i] * digit;
		digit /= 10;
	}

	if (isScoreDecision_) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 10; j++) {
				sprite2DNumResult_[i][j]->SetSize(easing_[i][j]->EaseOutElastic(
				    sprite2DNumResult_[i][j]->GetSize(), Vector2{1.0f, 1.0f}, spriteSize_[i][j], 60,
				    isEase_[i][j]));
			}
		}
	}

	if (!isDraw) {
		return;
	}
	sprite2DNumResult_[0][eachNumber[3]]->Draw();
	sprite2DNumResult_[1][eachNumber[2]]->Draw();
	sprite2DNumResult_[2][eachNumber[1]]->Draw();
	sprite2DNumResult_[3][eachNumber[0]]->Draw();
}