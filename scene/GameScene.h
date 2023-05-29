#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
//作成したクラスの読み込み
#include "Player.h"
#include "Enemy.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	
	bool isDebugCamera = false;
	DebugCamera* debugCamera_ = nullptr;

	//プレイヤーを包含
	Player* player_ = nullptr;
	//モデル
	Model* playerModel_ = nullptr;
	//テクスチャハンドル
	uint32_t playerTextureHandle_ = 0;

	//エネミーを包含
	Enemy* enemy_ = nullptr;
	//モデル
	Model* enemyModel_ = nullptr;
	Vector3 enemyGeneratePos_ = {};

	//ビュープロジェクション
	ViewProjection viewProjection_;

	/// <summary>
	/// 衝突判定
	/// </summary>
	void CheckAllCollision();

	/// <summary>
	/// ベクトルの距離を求める
	/// </summary>
	/// <param name="v"></param>
	/// <returns></returns>
	float Length(const Vector3& v);
};
