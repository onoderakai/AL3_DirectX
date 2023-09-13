#pragma once
#include "Scene.h"
#include "Input.h"
#include "Sprite.h"
#include "Easing.h"
#include "SoundManager.h"

class StageSelect {
public:
	/// <summary>
	/// コンストラク
	/// </summary>
	StageSelect();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~StageSelect();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(SceneNum* pScene);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void DrawBackground();

private:
	Input* input_ = nullptr;
	XINPUT_STATE joyState_;
	XINPUT_STATE preJoyState_;

	SoundManager* sound_ = nullptr;
	// シーン変数のポインタ
	SceneNum* pScene_ = nullptr;

	//イージング関連
	Easing* easing_ = nullptr;
	bool isEase_ = false;
	Vector2 start = {};
	Vector2 end = {};
	//ステージの数
	const uint32_t maxStageNum_ = 2;
	uint32_t stageNum_ = 0;
	uint32_t stageSelect_[2] = {};

	// 画像関連
	Sprite* stageSelectBgSprite_ = nullptr;
	Sprite* pushNextSprite_ = nullptr;

	Sprite* selectSprite_ = nullptr;
	Sprite* stageSprite_[2] = {};
	Sprite* arrowRightSprite_ = nullptr;
	Sprite* arrowLeftSprite_ = nullptr;
	Sprite* pushExplainSprite_ = nullptr;

	bool isExplain_ = false;
	Sprite* explainSprite_[3];
	// 説明画面が出ているときの背景
	Sprite* backGround_ = nullptr;
};
