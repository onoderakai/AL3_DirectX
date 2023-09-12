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
#include "IChatch.h"
#include "Score.h"
#include "Finish.h"
#include "GameResult.h"
#include "SoundManager.h"

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
	SceneNum scene_ = SceneNum::SCORE_ATTACK_STAGE;
	Title* title_ = nullptr;
	//Stage* stage_ = nullptr;
	Clear* clear_ = nullptr;
	GameOver* gameOver_ = nullptr;
	OperationExplain* explain_ = nullptr;
	StageSelect* stageSelect_ = nullptr;
	
	//一列の達磨の最大数
	const uint32_t kMaxDaruma_ = 12;
	//達磨の最大数
	const uint32_t kMaxDarumaNum_ = 3;
	Score* score_ = nullptr;
	IChatch* iChatch_ = nullptr;
	Finish* finish_ = nullptr;
	GameResult* result_ = nullptr;

	//達磨モデル
	Model* darumaGreenModel_ = nullptr;
	Model* darumaRedModel_ = nullptr;
	Model* darumaBlueModel_ = nullptr;
	Model* darumaYellowModel_ = nullptr;
	Model* darumaTopModel_ = nullptr;

	Daruma* daruma_[3][12];
	DarumaType darumaType_[3][12] = {};

	//LB、RBの画像
	Sprite* LBSprite_ = nullptr;
	Sprite* RBSprite_ = nullptr;

	//達磨の初期位置を保存
	Vector3 startDarumaPos[12] = {};

	//何列目の達磨を選択してるかの変数
	uint32_t darumaNum_ = 0;

	//壊れた達磨の数
	uint32_t preDarumaCount_[3] = {};
	uint32_t darumaCount_[3] = {};

	//選択している達磨のオフセット値
	const float kSelectDarumaZ = -40.0f;
	// 選択していない達磨のオフセット値
	const float kNoSelectDarumaZ = -30.0f;

	//一列の達磨同士の間隔
	const float kDarumaRowSpacingY = 5.0f;

	//達磨を生成するときのオフセット値
	const Vector3 kDarumaOffset = {-30.0f, -20.0f, 0.0f};

	//スコア
	uint32_t scorePoint_ = 0;
	//経過時間
	uint32_t timeCount_ = 0;
	const uint32_t kMaxScoreModeTime_ = 900;
	int32_t scoreModeTimeCount_ = kMaxScoreModeTime_;

	bool isMakeFinish_ = false;
	const uint32_t kMaxMakeFinishCount_ = 30;
	uint32_t makeFinishCount_ = kMaxMakeFinishCount_;

	//ミスタッチをしたときのペナルティタイム
	int32_t penaltyTime_ = 0;

	//行消ししたときのフラグ
	bool isRowBreak_ = false;

	XINPUT_STATE joyState_;
	XINPUT_STATE preJoyState_;

	//達磨のcsv
	std::stringstream darumaPopCommands_;
	
	bool isDebugCamera = false;
	DebugCamera* debugCamera_ = nullptr;

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

	void ScoreAttackUpdate();

	void TimeAttackUpdate();

	/// <summary>
	/// 指定した列かつ、指定した行を一番上に持っていき、それより上の行を下に詰める
	/// </summary>
	/// <param name="darumaColumn">列</param>
	/// <param name="darumaRow">行</param>
	void StackArray(uint32_t darumaColumn, uint32_t darumaRow);

	/// <summary>
	/// 達磨の選択列を移動
	/// </summary>
	/// <param name="move">移動量(+1なら右に移動。-1なら左に移動。)</param>
	void SelectDarumaChange(int32_t move);
};
