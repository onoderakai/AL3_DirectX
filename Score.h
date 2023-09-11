#pragma once
#include <Model.h>
#include <Sprite.h>
#include "Easing.h"

class Score {
public:
	/// <summary>
	/// コンストラク
	/// </summary>
	Score();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Score();
	/// <summary>
	/// initialize
	/// </summary>
	void Initialize();

	void DrawScoreUI(int score);
	void DrawScoreUIResult(int score);

	/// <summary>
	/// スコアが決定したフラグのゲッター
	/// </summary>
	/// <returns></returns>
	bool GetIsScoreDecision() { return isScoreDecision_; }

private:
	Easing* easing_[4][10];

	Sprite* sprite2DNum_[4][10];
	Sprite* sprite2DNumResult_[4][10];

	Vector2 spriteSize_[4][10] = {};

	//スコアが決定したときのフラグ
	bool isScoreDecision_ = false;
	bool isEase_[4][10] = {};

	int32_t tmpScore_ = 0;
	int32_t preScore_ = 0;
	int32_t addTmpScore_ = 0;
};