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
#include "OperationExplain.h"
#include "StageSelect.h"
#include "Scene.h"
#include "ParticleSystem.h"

#include "EnemyType.h"
#include "Daruma.h"
#include "DarumaType.h"

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

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	
	//シーン関連
	SceneNum scene_ = SceneNum::STAGE;
	Title* title_ = nullptr;
	//Stage* stage_ = nullptr;
	Clear* clear_ = nullptr;
	GameOver* gameOver_ = nullptr;
	OperationExplain* explain_ = nullptr;
	StageSelect* stageSelect_ = nullptr;
	SceneNum preScene_ = scene_;

	const uint32_t kMaxDaruma_ = 10;
	const uint32_t kMaxDarumaNum_ = 3;
	Model* darumaGreenModel_ = nullptr;
	Model* darumaRedModel_ = nullptr;
	Model* darumaBlueModel_ = nullptr;
	Model* darumaYellowModel_ = nullptr;
	Daruma* daruma_[3][10];
	DarumaType darumaType_[3][10] = {};

	Vector3 startDarumaPos[10] = {};

	uint32_t darumaNum_ = 0;
	uint32_t preDarumaCount_[3] = {};
	uint32_t darumaCount_[3] = {};
	//達磨のcsv
	std::stringstream darumaPopCommands_;

	std::stringstream stage1EnemyPopCommands;
	//待機中のフラグ
	bool isWait_ = false;
	//待機中の時間
	int32_t waitTime_ = 0;
	//撃破フラグ
	bool isDefeat_ = false;
	
	bool isDebugCamera = false;
	DebugCamera* debugCamera_ = nullptr;

	//モデル
	Model* playerModel_ = nullptr;
	Model* playerSniperModel_ = nullptr;
	
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

	void LoadDarumaPopData();

	void UpdateDarumaPopCommands();

	/// <summary>
	/// 敵発生コマンドの更新処理
	/// </summary>
	void UpdateEnemyPopCommands();

	void StageUpdate();

	/// <summary>
	/// 指定した列かつ、指定した行を一番上に持っていき、それより上の行を下に詰める
	/// </summary>
	/// <param name="darumaColumn">列</param>
	/// <param name="darumaRow">行</param>
	void StackArray(uint32_t darumaColumn, uint32_t darumaRow);
};
