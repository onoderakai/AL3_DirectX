﻿#pragma once
#include "Scene.h"
#include "Sprite.h"
#include "Input.h"
#include "SoundManager.h"

class GameScene;

class SceneChange final{
public:
	/// <summary>
	/// コピーコンストラクタの禁止
	/// </summary>
	/// <param name="sceneChange"></param>
	SceneChange(const SceneChange& sceneChange) = delete;

	/// <summary>
	/// 代入演算子の禁止
	/// </summary>
	/// <param name="sceneChange"></param>
	/// <returns></returns>
	SceneChange& operator=(const SceneChange& sceneChange) = delete;


	/// <summary>
	/// インスタンスの取得
	/// </summary>
	/// <returns></returns>
	static SceneChange* GetInstance();

	/// <summary>
	/// シーンの遷移
	/// </summary>
	/// <param name="nextScene"></param>
	/// <param name="pScene"></param>
	void Change(SceneNum nextScene, SceneNum* pScene);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	void Update2();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// シーン遷移中のフラグのゲッター
	/// </summary>
	/// <returns></returns>
	bool GetIsLoading() { return isLoading_; }

	/// <summary>
	/// ゲームシーンのセッター
	/// </summary>
	/// <param name="gameScene"></param>
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

private:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	SceneChange();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SceneChange();

	GameScene* gameScene_ = nullptr;
	SoundManager* sound_ = nullptr;

	//遷移フラグ
	bool isLoading_ = false;
	//遷移に必要なフレーム数
	uint32_t transitionFrame_;
	//遷移中のフレームカウント
	uint32_t frameCount_ = 0;
	//シーンのポインタ
	SceneNum* pScene_;
	//遷移先のシーン
	SceneNum nextScene_;
	//遷移画像
	uint32_t loadingTextureHandle_ = 0;
	Sprite* loadingSprite_ = nullptr;

	Sprite* loadingSprite2_ = nullptr;
	//1フレームの移動量
	float moveValue_ = 0.0f;
	bool isMoveIn_ = false;

	//遷移画像の色
	Vector4 color = {0.0f, 0.0f, 0.0f, 0.0f};
	float addAlpha_ = 0.0f;

	Input* input_ = nullptr;
};
