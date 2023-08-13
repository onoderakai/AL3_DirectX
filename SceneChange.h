#pragma once
#include "Scene.h"
#include "Sprite.h"

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

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	SceneChange();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SceneChange();

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
	//遷移画像の色
	Vector4 color = {0.0f, 0.0f, 0.0f, 0.0f};
	float addAlpha_ = 0.0f;
};
