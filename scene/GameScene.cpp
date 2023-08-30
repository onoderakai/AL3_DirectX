#include "GameScene.h"
#include "AxisIndicator.h"
#include "ImGuiManager.h"
#include "MathUtility.h"
#include "SceneChange.h"
#include "TextureManager.h"
#include <cassert>
#include <cmath>
#include <fstream>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete particleSystem_;
	delete player_;
	delete playerModel_;
	delete playerSniperModel_;
	for (Enemy* enemy : enemys_) {
		delete enemy;
	}
	for (EnemyBullet* bullet : enemyBullets_) {
		delete bullet;
	}
	delete enemyModel_;
	delete boss_;
	delete bossModel_;
	delete skydomeModel_;
	delete skydome_;
	delete railCamera_;
	delete title_;
	// delete stage_;
	delete clear_;
	delete gameOver_;
	delete explain_;
	delete stageSelect_;

	delete debugCamera_;
}

void GameScene::Initialize() {

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
#ifdef _DEBUG
	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
#endif // _DEBUG

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// パーティクルシステムの生成
	particleSystem_ = new ParticleSystem();
	particleSystem_->Initialize();

	// プレイヤーモデルの生成
	playerModel_ = Model::CreateFromOBJ("Player", true);
	playerSniperModel_ = Model::CreateFromOBJ("SniperPlayer", true);

	// エネミーモデルの生成
	enemyModel_ = Model::CreateFromOBJ("Enemy1", true);

	// ボスモデルの生成
	bossModel_ = Model::CreateFromOBJ("Boss", true);

	// 天球のモデルを生成
	skydomeModel_ = Model::CreateFromOBJ("skydome", true);

	// インスタンスの生成
	// プレイヤー
	player_ = new Player();
	Vector3 playerPos = {0.0f, -7.0f, 20.0f};
	player_->Initialeze(playerModel_, playerSniperModel_, playerPos);
	player_->SetEnemys(enemys_);
	player_->SetParticleSystem(particleSystem_);

	// ボス
	boss_ = new Boss();
	boss_->SetPlayer(player_);
	boss_->Initialize(bossModel_);
	boss_->SetParticleSystem(particleSystem_);

	// 天球
	skydome_ = new Skydome();
	skydome_->Initialize(skydomeModel_);

	// レールカメラ
	railCamera_ = new RailCamera();
	railCamera_->Initialize({0.0f, 0.0f, -100.0f}, {0.0f, 0.0f, 0.0f});

	// 親子関係を結ぶ
	player_->SetParent(&railCamera_->GetWorldTransform());

	// ビュープロジェクションの初期化
	viewProjection_.farZ = 100.0f;
	viewProjection_.Initialize();

	// ファイル読み込み
	LoadEnemyPopData();

	// レティクル画像読み込み
	TextureManager::Load("target.png");

	// シーン関連の生成
	title_ = new Title();
	title_->Initialize(&scene_);
	clear_ = new Clear();
	clear_->Initialize(&scene_);
	gameOver_ = new GameOver();
	gameOver_->Initialize(&scene_);
	explain_ = new OperationExplain();
	explain_->Initialize(&scene_);
	stageSelect_ = new StageSelect();
	stageSelect_->Initialize(&scene_);
}

void GameScene::SceneInitialize() {
	// ファイルの読み込み行を0行目に戻す
	stage1EnemyPopCommands.clear();
	stage1EnemyPopCommands.seekg(0, ios::beg);

	particleSystem_->Initialize();

	enemyBullets_.remove_if([](EnemyBullet* bullet) {
		delete bullet;
		return true;
	});

	enemys_.remove_if([](Enemy* enemy) {
		delete enemy;
		return true;
	});

	Vector3 playerPos = {0.0f, -7.0f, 20.0f};
	player_->Initialeze(playerModel_, playerSniperModel_, playerPos);
	player_->SetEnemys(enemys_);
	player_->SetParticleSystem(particleSystem_);

	boss_->Initialize(bossModel_);

	skydome_->Initialize(skydomeModel_);

	railCamera_->Initialize({0.0f, 0.0f, -100.0f}, {0.0f, 0.0f, 0.0f});

	// 親子関係を結ぶ
	player_->SetParent(&railCamera_->GetWorldTransform());

	// ビュープロジェクションの初期化
	viewProjection_.farZ = 100.0f;
	viewProjection_.Initialize();

	title_->Initialize(&scene_);
	clear_->Initialize(&scene_);
	gameOver_->Initialize(&scene_);
	explain_->Initialize(&scene_);
	stageSelect_->Initialize(&scene_);

	// コマンドの初期化
	isDefeat_ = false;
	isWait_ = false;
	waitTime_ = 0;
}

void GameScene::Update() {
#ifdef _DEBUG
	// デバッグカメラの処理
	// デバッグカメラのアクティブ切り替え
	if (input_->TriggerKey(DIK_B)) {
		isDebugCamera = !isDebugCamera;
	}

	// デバッグカメラの処理
	if (isDebugCamera) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	}
#endif // _DEBUG
	preScene_ = scene_;
	// 1でTITLEシーンに遷移する
	if (input_->PushKey(DIK_1)) {
		SceneChange::GetInstance()->Change(SceneNum::TITLE, &scene_);
	}
	// 2でSTAGEシーンに遷移する
	else if (input_->PushKey(DIK_2)) {
		SceneChange::GetInstance()->Change(SceneNum::STAGE, &scene_);
	}
	// 3でBOSS_STAGEシーンに遷移する
	else if (input_->PushKey(DIK_3)) {
		SceneChange::GetInstance()->Change(SceneNum::BOSS_STAGE, &scene_);
	}
	// 4でCLEARシーンに遷移する
	else if (input_->PushKey(DIK_4)) {
		SceneChange::GetInstance()->Change(SceneNum::CLEAR, &scene_);
	}
	// 5でGAMEOVERシーンに遷移する
	else if (input_->PushKey(DIK_5)) {
		SceneChange::GetInstance()->Change(SceneNum::GAMEOVER, &scene_);
	} else if (input_->PushKey(DIK_6)) {
		stage1EnemyPopCommands.clear();
		stage1EnemyPopCommands.seekg(0, ios::beg);
	}

	switch (scene_) {
	case SceneNum::TITLE:
		title_->Update();
		break;
	case SceneNum::EXPLAIN:
		explain_->Update();
		break;
	case SceneNum::STAGE_SELECT:
		stageSelect_->Update();
		break;
	case SceneNum::STAGE:
		UpdateEnemyPopCommands();

		railCamera_->Update();
		if (!isDebugCamera) {
			viewProjection_.matView = railCamera_->GetViewProjection().matView;
			viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
			viewProjection_.TransferMatrix();
		}

		// プレイヤーの更新処理
		if (player_) {
			if (player_->GetIsDead()) {
				gameOver_->SetPreScene(SceneNum::STAGE);
				SceneChange::GetInstance()->Change(SceneNum::GAMEOVER, &scene_);
			} else {
				player_->Update(viewProjection_);
			}
		}
		// デスフラグがtrueの敵を削除する
		enemys_.remove_if([](Enemy* enemy) {
			if (enemy->GetIsDead()) {
				delete enemy;
				return true;
			}
			return false;
		});
		// プレイヤーに敵の情報を渡す
		player_->SetEnemys(enemys_);
		// 敵の更新処理を呼ぶ
		for (Enemy* enemy : enemys_) {
			if (enemy) {
				enemy->Update();
			}
		}
		// デスフラグがtrueの弾を削除する
		enemyBullets_.remove_if([](EnemyBullet* bullet) {
			if (bullet->GetIsDead()) {
				delete bullet;
				return true;
			}
			return false;
		});
		// 弾の更新処理を呼ぶ
		for (EnemyBullet* bullet : enemyBullets_) {
			bullet->Update();
		}
		// 天球の更新処理
		if (skydome_) {
			skydome_->Update();
		}
		// パーティクルシステムの更新処理
		particleSystem_->Update();
		// 衝突判定
		CheckAllCollision();
		break;
	case SceneNum::BOSS_STAGE:
		railCamera_->Update();
		if (!isDebugCamera) {
			viewProjection_.matView = railCamera_->GetViewProjection().matView;
			viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
			viewProjection_.TransferMatrix();
		}
		// プレイヤーの更新処理
		if (player_) {
			if (player_->GetIsDead()) {
				gameOver_->SetPreScene(SceneNum::BOSS_STAGE);
				SceneChange::GetInstance()->Change(SceneNum::GAMEOVER, &scene_);
			} else {
				player_->Update(viewProjection_);
			}
		}
		// ボスの更新処理
		if (boss_) {
			if (boss_->GetIsDead()) {
				SceneChange::GetInstance()->Change(SceneNum::CLEAR, &scene_);
			} else {
				boss_->Update();
			}
		}
		// 天球の更新処理
		if (skydome_) {
			skydome_->Update();
		}
		// パーティクルシステムの更新処理
		particleSystem_->Update();
		// 衝突判定
		CheckAllCollision();
		break;
	case SceneNum::CLEAR:
		clear_->Update();
		break;
	case SceneNum::GAMEOVER:
		gameOver_->Update();
		break;
	default:
		break;
	}
	SceneChange::GetInstance()->Update();
	if (preScene_ != scene_) {
		SceneInitialize();
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	switch (scene_) {
	case SceneNum::TITLE:

		break;
	case SceneNum::STAGE:
		if (player_) {
			if (!player_->GetIsDead()) {
				player_->Draw(viewProjection_);
			}
		}
		for (EnemyBullet* bullet : enemyBullets_) {
			bullet->Draw(viewProjection_);
		}
		for (Enemy* enemy : enemys_) {
			if (enemy) {
				enemy->Draw(viewProjection_);
			}
		}
		if (skydome_) {
			skydome_->Draw(viewProjection_);
		}
		particleSystem_->Draw(viewProjection_);
		break;
	case SceneNum::BOSS_STAGE:
		if (player_) {
			if (!player_->GetIsDead()) {
				player_->Draw(viewProjection_);
			}
		}
		if (boss_) {
			boss_->Draw(viewProjection_);
		}
		if (skydome_) {
			skydome_->Draw(viewProjection_);
		}
		particleSystem_->Draw(viewProjection_);
		break;
	default:
		break;
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	switch (scene_) {
	case SceneNum::TITLE:
		title_->DrawBackground();
		break;
	case SceneNum::EXPLAIN:
		explain_->DrawBackground();
		break;
	case SceneNum::STAGE_SELECT:
		stageSelect_->DrawBackground();
		break;
	case SceneNum::STAGE:
		player_->DrawUI();
		break;
	case SceneNum::BOSS_STAGE:
		player_->DrawUI();
		boss_->DrawUI();
		break;
	case SceneNum::CLEAR:
		clear_->DrawBackground();
		break;
	case SceneNum::GAMEOVER:
		gameOver_->DrawBackground();
		break;
	default:
		break;
	}

	// 遷移画面の描画
	SceneChange::GetInstance()->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollision() {
	// コライダーリスト
	list<Collider*> colliders;

	// 衝突判定を行うために、コライダークラスを継承したクラスをコライダーリストに追加する
	if (!player_->GetIsDead()) {
		colliders.push_back(player_);
	}
	if (scene_ == SceneNum::BOSS_STAGE) {
		if (!boss_->GetIsDead()) {
			colliders.push_back(boss_);
			for (BossBullet* bullet : boss_->GetBullets()) {
				colliders.push_back(bullet);
			}
		}
	}
	for (Enemy* enemy : enemys_) {
		colliders.push_back(enemy);
	}

	for (PlayerBullet* playerBullet : player_->GetBullets()) {
		colliders.push_back(playerBullet);
	}
	for (EnemyBullet* enemyBullet : enemyBullets_) {
		colliders.push_back(enemyBullet);
	}

	// コライダーリスト内のオブジェクトを総当たりで衝突判定する
	list<Collider*>::iterator itrA = colliders.begin();
	for (; itrA != colliders.end(); itrA++) {
		Collider* A = *itrA;
		list<Collider*>::iterator itrB = itrA;
		itrB++;
		for (; itrB != colliders.end(); itrB++) {
			Collider* B = *itrB;
			CheckCollisionPair(A, B);
		}
	}
}

void GameScene::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {
	// 衝突フィルタリング
	if (((colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask()) == 0) ||
	    ((colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask())) == 0) {
		return;
	}

	// 座標の保存
	Vector3 posA = colliderA->GetWorldPosition();
	Vector3 posB = colliderB->GetWorldPosition();

	// 距離を求める
	float length = Length(posA - posB);
	// 半径同士の足し算
	float radius = colliderA->GetRadius() + colliderB->GetRadius();
	if (length <= radius) {
		colliderA->OnCollision();
		colliderB->OnCollision();
	}
}

void GameScene::LoadEnemyPopData() {
	// ファイル読み込み
	ifstream file;
	file.open("./Resources/enemyPop.csv");
	assert(file.is_open());
	// ファイルをコピー
	stage1EnemyPopCommands << file.rdbuf();
	// ファイルを閉じる
	file.close();
}

void GameScene::UpdateEnemyPopCommands() {
	// 待機処理
	if (isDefeat_) {
		uint32_t enemyCount = 0;
		for (Enemy* enemy : enemys_) {
			enemy;
			enemyCount++;
		}
		if (enemyCount <= 0) {
			isDefeat_ = false;
		}
		return;
	}

	if (isWait_) {
		waitTime_--;
		if (waitTime_ <= 0) {
			isWait_ = false;
		}
		return;
	}

	// 1行分の文字列を入れる変数
	string line;

	// コマンド実行ループ
	while (getline(stage1EnemyPopCommands, line)) {

		istringstream line_stream(line);

		string word;
		getline(line_stream, word, ',');

		// //から始まる行はコメントのため飛ばす
		if (word.find("//") == 0) {
			// コメント行を飛ばす
			continue;
		}

		if (word.find("POP") == 0) {
			// X座標
			getline(line_stream, word, ',');
			float x = (float)atof(word.c_str());
			// Y座標
			getline(line_stream, word, ',');
			float y = (float)atof(word.c_str());
			// Z座標
			getline(line_stream, word, ',');
			float z = (float)atof(word.c_str());

			// 敵のタイプ
			Type enemyType = Type::NORMAL;
			getline(line_stream, word, ',');
			if (word.find("NORMAL") == 0) {
				enemyType = Type::NORMAL;
			} else if (word.find("TO_PLAYER") == 0) {
				enemyType = Type::TO_PLAYER;
			} else if (word.find("HOMING") == 0) {
				enemyType = Type::HOMING;
			}
			AddEnemy(enemyType, {x, y, z});
		} else if (word.find("WAIT") == 0) {

			getline(line_stream, word, ',');

			int32_t waitTime = atoi(word.c_str());

			isWait_ = true;
			waitTime_ = waitTime;

			// コマンドループを抜ける
			break;
		} else if (word.find("DEFEAT") == 0) {
			getline(line_stream, word, ',');
			isDefeat_ = true;

			// コマンドループを抜ける
			break;
		} else if (word.find("CLEAR") == 0) {
			getline(line_stream, word, ',');
			SceneChange::GetInstance()->Change(SceneNum::CLEAR, &scene_);
			// コマンドループを抜ける
			break;
		}
	}
}

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {
	// リストに追加する
	enemyBullets_.push_back(enemyBullet);
}

void GameScene::AddEnemy(Type type, Vector3 pos) {
	Enemy* newEnemy = new Enemy();
	newEnemy->SetPlayer(player_);
	newEnemy->SetGameScene(this);
	newEnemy->SetParticleSystem(particleSystem_);
	newEnemy->Initialize(type, enemyModel_, pos);
	enemys_.push_back(newEnemy);
}
