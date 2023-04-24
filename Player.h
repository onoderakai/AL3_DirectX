#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"

/// <summary>
/// 自キャラのクラス
/// </summary>
class Player {
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル情報</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialeze(Model* model, uint32_t textureHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Uppdate();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(ViewProjection& viewProjection);

private:
	//入力
	Input* input_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル情報
	Model* model_ = nullptr;
};
