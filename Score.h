#pragma once
#include <Model.h>
#include <Sprite.h>

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

private:
	Sprite* sprite2DNum_[4][10];
	Sprite* sprite2DNumResult_[4][10];

	int32_t tmpScore_ = 0;
	int32_t preScore_ = 0;
	int32_t addTmpScore_ = 0;
};