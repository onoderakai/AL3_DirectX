#pragma once
#include "Scene.h"
#include "Input.h"
#include "Sprite.h"
#include "Easing.h"
#include "SoundManager.h"

class Title {
public:
	/// <summary>
	/// コンストラク
	/// </summary>
	Title();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Title();

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
	// ジョイスティックを使う
	XINPUT_STATE joyState_ = {};
	XINPUT_STATE preJoyState_ = {};

	SoundManager* sound_ = nullptr;

	//タイトル画像をイージングさせる
	Easing* titleEasing_ = nullptr;
	Easing* titleCatEasing_ = nullptr;
	bool isTitleEase_ = true;
	Vector2 titleStartPos_ = {};
	Vector2 tmpTitleStartPos_ = {};
	Vector2 titleEndPos_ = {};
	Vector2 tmpTitleEndPos_ = {};
	bool isTitleCatEase_ = true;
	bool isTitleCatEaseChange_ = false;
	Vector2 titleCatStartPos_ = {};
	Vector2 tmpTitleCatStartPos_ = {};
	Vector2 titleCatEndPos_ = {};
	Vector2 tmpTitleCatEndPos_ = {};

	//シーン変数のポインタ
	SceneNum* pScene_ = nullptr;

	//タイトル画像関連
	uint32_t titleTextureHandle_ = 0;
	Sprite* titleSprite_ = nullptr;
	Sprite* titleCatSprite_ = nullptr;
	Sprite* titleBgSprite_ = nullptr;
	Sprite* titleDarumaSprite_ = nullptr;
	//次に進む
	Sprite* pushNextSprite_ = nullptr;

	bool isChange_ = false;
	float flashSpeed_ = -0.02f;
};
