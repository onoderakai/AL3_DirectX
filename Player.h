﻿#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "PlayerBullet.h"
#include <list>

/// <summary>
/// 自キャラのクラス
/// </summary>
class Player {
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player() {
		for (PlayerBullet* bullet : bullets_) {
			delete bullet;
		}
	}

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル情報</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialeze(Model* model, uint32_t textureHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(ViewProjection& viewProjection);

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	//ゲッター
	
	//セッター

private:
	//入力
	Input* input_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// ワールド変換データ
	WorldTransform world_;
	// モデル情報
	Model* model_ = nullptr;

	//弾
	std::list<PlayerBullet*> bullets_;
};
