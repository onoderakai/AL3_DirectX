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
#include <sstream>
//作成したクラスの読み込み
#include "Player.h"
#include "Enemy.h"
#include "Skydome.h"
#include "RailCamera.h"

using namespace std;

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
	
	/// <summary>
	/// 敵弾を追加する
	/// </summary>
	/// <param name="enemyBullet"></param>
	void AddEnemyBullet(EnemyBullet* enemyBullet);

	/// <summary>
	/// 敵を追加する
	/// </summary>
	/// <param name="pos"></param>
	void AddEnemy(Vector3 pos);

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	
	std::stringstream enemyPopCommands;
	//待機中のフラグ
	bool isWait_ = false;
	//待機中の時間
	int32_t waitTime_ = 0;

	bool isDebugCamera = false;
	DebugCamera* debugCamera_ = nullptr;

	//プレイヤーを包含
	Player* player_ = nullptr;
	//モデル
	Model* playerModel_ = nullptr;
	//テクスチャハンドル
	uint32_t playerTextureHandle_ = 0;

	//エネミーを包含
	//Enemy* enemy_ = nullptr;
	std::list<Enemy*> enemys_;
	// 弾の包含
	std::list<EnemyBullet*> enemyBullets_;
	//モデル
	Model* enemyModel_ = nullptr;
	Vector3 enemyGeneratePos_ = {};

	//天球を包含
	Skydome* skydome_ = nullptr;
	//モデル
	Model* skydomeModel_ = nullptr;

	//ビュープロジェクション
	ViewProjection viewProjection_;

	//レールカメラの包含
	RailCamera* railCamera_ = nullptr;

	/// <summary>
	/// 衝突判定
	/// </summary>
	void CheckAllCollision();

	/// <summary>
	/// コライダー2つの衝突判定と応答
	/// </summary>
	/// <param name="colliderA"></param>
	/// <param name="colliderB"></param>
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);

	/// <summary>
	/// ベクトルの距離を求める
	/// </summary>
	/// <param name="v"></param>
	/// <returns></returns>
	float Length(const Vector3& v);

	/// <summary>
	/// 敵の出現データを保存
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 敵発生コマンドの更新処理
	/// </summary>
	void UpdateEnemyPopCommands();
};
