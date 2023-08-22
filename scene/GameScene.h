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
#include "Boss.h"
#include "Enemy.h"
#include "Skydome.h"
#include "RailCamera.h"
#include "Title.h"
//#include "Stage.h"
#include "Clear.h"
#include "GameOver.h"
#include "Scene.h"
#include "ParticleSystem.h"

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
	/// 初期化
	/// </summary>
	void SceneInitialize();

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
	
	//シーン関連
	SceneNum scene_ = SceneNum::TITLE;
	Title* title_ = nullptr;
	//Stage* stage_ = nullptr;
	Clear* clear_ = nullptr;
	GameOver* gameOver_ = nullptr;
	SceneNum preScene_ = scene_;

	std::stringstream stage1EnemyPopCommands;
	//待機中のフラグ
	bool isWait_ = false;
	//待機中の時間
	int32_t waitTime_ = 0;
	//撃破フラグ
	bool isDefeat_ = false;

	bool isDebugCamera = false;
	DebugCamera* debugCamera_ = nullptr;

	//プレイヤーを包含
	Player* player_ = nullptr;
	//モデル
	Model* playerModel_ = nullptr;
	
	//エネミーを包含
	std::list<Enemy*> enemys_;
	// 弾の包含
	std::list<EnemyBullet*> enemyBullets_;
	//モデル
	Model* enemyModel_ = nullptr;
	Vector3 enemyGeneratePos_ = {};

	//ボスを包含
	Boss* boss_ = nullptr;
	// モデル
	Model* bossModel_ = nullptr;
	// テクスチャハンドル
	uint32_t bossTextureHandle_ = 0;

	//天球を包含
	Skydome* skydome_ = nullptr;
	//モデル
	Model* skydomeModel_ = nullptr;

	//ビュープロジェクション
	ViewProjection viewProjection_;

	//レールカメラの包含
	RailCamera* railCamera_ = nullptr;

	// パーティクルシステムの包含
	ParticleSystem* particleSystem_ = nullptr;


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
	/// 敵の出現データを保存
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 敵発生コマンドの更新処理
	/// </summary>
	void UpdateEnemyPopCommands();
};
