#include "GameScene.h"
#include "AxisIndicator.h"
#include "ImGuiManager.h"
#include "MathUtility.h"
#include "SceneChange.h"
#include "TextureManager.h"
#include <cassert>
#include <cmath>
#include <fstream>

GameScene::GameScene() { LoadDarumaPopData(); }

GameScene::~GameScene() {
	for (uint32_t i = 0; i < kMaxDarumaNum_; i++) {
		for (uint32_t j = 0; j < kMaxDaruma_; j++) {
			delete daruma_[i][j];
		}
	}

	delete darumaGreenModel_;
	delete darumaRedModel_;
	delete darumaBlueModel_;
	delete darumaYellowModel_;

	delete particleSystem_;
	delete playerModel_;
	delete playerSniperModel_;
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

	// 達磨モデルの生成
	darumaGreenModel_ = Model::CreateFromOBJ("DarumaGreen", true);
	darumaRedModel_ = Model::CreateFromOBJ("DarumaRed", true);
	darumaBlueModel_ = Model::CreateFromOBJ("DarumaBlue", true);
	darumaYellowModel_ = Model::CreateFromOBJ("DarumaYellow", true);

	// ボスモデルの生成
	bossModel_ = Model::CreateFromOBJ("Boss", true);

	// 天球のモデルを生成
	skydomeModel_ = Model::CreateFromOBJ("skydome", true);

	// インスタンスの生成
	// 天球
	skydome_ = new Skydome();
	skydome_->Initialize(skydomeModel_);

	// レールカメラ
	railCamera_ = new RailCamera();
	railCamera_->Initialize({0.0f, 0.0f, -100.0f}, {0.0f, 0.0f, 0.0f});

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

	for (uint32_t i = 0; i < kMaxDarumaNum_; i++) {
		for (uint32_t j = 0; j < kMaxDaruma_; j++) {
			daruma_[i][j] = new Daruma();
			daruma_[i][j]->Initialize(
			    darumaGreenModel_, Vector3{0.0f, j * 5.0f - 10.0f, i * 120.0f - 30.0f}, DarumaType::GREEN);
		}
	}
	for (uint32_t i = 0; i < kMaxDaruma_; i++) {
		startDarumaPos[i] = daruma_[0][i]->GetWorldPosition();
	}

	UpdateDarumaPopCommands();
}

void GameScene::SceneInitialize() {
	// ファイルの読み込み行を0行目に戻す
	stage1EnemyPopCommands.clear();
	stage1EnemyPopCommands.seekg(0, ios::beg);

	particleSystem_->Initialize();

	skydome_->Initialize(skydomeModel_);

	railCamera_->Initialize({0.0f, 0.0f, -100.0f}, {0.0f, 0.0f, 0.0f});

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
	if (input_->TriggerKey(DIK_O)) {
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
#ifdef _DEBUG
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
#endif // _DEBUG

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
		// UpdateEnemyPopCommands();

		railCamera_->Update();
		if (!isDebugCamera) {
			viewProjection_.matView = railCamera_->GetViewProjection().matView;
			viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
			viewProjection_.TransferMatrix();
		}

		StageUpdate();

		//// プレイヤーの更新処理
		// if (player_) {
		//	if (player_->GetIsDead()) {
		//		gameOver_->SetPreScene(SceneNum::STAGE);
		//		SceneChange::GetInstance()->Change(SceneNum::GAMEOVER, &scene_);
		//	} else {
		//		player_->Update(viewProjection_);
		//	}
		// }
		//// デスフラグがtrueの敵を削除する
		// enemys_.remove_if([](Enemy* enemy) {
		//	if (enemy->GetIsDead()) {
		//		delete enemy;
		//		return true;
		//	}
		//	return false;
		// });
		//// プレイヤーに敵の情報を渡す
		// player_->SetEnemys(enemys_);
		//// 敵の更新処理を呼ぶ
		// for (Enemy* enemy : enemys_) {
		//	if (enemy) {
		//		enemy->Update();
		//	}
		// }
		//// デスフラグがtrueの弾を削除する
		// enemyBullets_.remove_if([](EnemyBullet* bullet) {
		//	if (bullet->GetIsDead()) {
		//		delete bullet;
		//		return true;
		//	}
		//	return false;
		// });
		//// 弾の更新処理を呼ぶ
		// for (EnemyBullet* bullet : enemyBullets_) {
		//	bullet->Update();
		// }
		//  天球の更新処理
		if (skydome_) {
			skydome_->Update();
		}
		// パーティクルシステムの更新処理
		particleSystem_->Update();
		//// 衝突判定
		// CheckAllCollision();
		break;
	case SceneNum::BOSS_STAGE:
		railCamera_->Update();
		if (!isDebugCamera) {
			viewProjection_.matView = railCamera_->GetViewProjection().matView;
			viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
			viewProjection_.TransferMatrix();
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
		for (uint32_t i = 0; i < kMaxDarumaNum_; i++) {
			for (uint32_t j = 0; j < kMaxDaruma_; j++) {
				daruma_[i][j]->Draw(viewProjection_);
			}
		}

		if (skydome_) {
			skydome_->Draw(viewProjection_);
		}
		particleSystem_->Draw(viewProjection_);
		break;
	case SceneNum::BOSS_STAGE:
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
		/*player_->DrawUI();*/
		break;
	case SceneNum::BOSS_STAGE:

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
	if (scene_ == SceneNum::BOSS_STAGE) {
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

void GameScene::LoadDarumaPopData() {
	// ファイル読み込み
	ifstream file;
	file.open("./Resources/darumaPop.csv");
	assert(file.is_open());
	// ファイルをコピー
	darumaPopCommands_ << file.rdbuf();
	// ファイルを閉じる
	file.close();
}

void GameScene::UpdateDarumaPopCommands() {
	// 1行分の文字列を入れる変数
	string line;
	uint32_t i = 0;

	// コマンド実行ループ
	while (getline(darumaPopCommands_, line)) {

		istringstream line_stream(line);

		string word;
		getline(line_stream, word, ',');

		// //から始まる行はコメントのため飛ばす
		if (word.find("//") == 0) {
			// コメント行を飛ばす
			continue;
		}

		if (i >= kMaxDaruma_) {
			break;
		}
		if (word.find("RED") == 0) {
			getline(line_stream, word, ',');
			daruma_[0][i]->Initialize(
			    darumaRedModel_, Vector3{0.0f, i * 5.0f - 10.0f, -30.0f}, DarumaType::RED);
			i++;
		} else if (word.find("BLUE") == 0) {
			getline(line_stream, word, ',');
			daruma_[0][i]->Initialize(
			    darumaBlueModel_, Vector3{0.0f, i * 5.0f - 10.0f, -30.0f}, DarumaType::BLUE);
			i++;
		} else if (word.find("GREEN") == 0) {
			getline(line_stream, word, ',');
			daruma_[0][i]->Initialize(
			    darumaGreenModel_, Vector3{0.0f, i * 5.0f - 10.0f, -30.0f}, DarumaType::GREEN);
			i++;
		} else if (word.find("YELLOW") == 0) {
			getline(line_stream, word, ',');
			daruma_[0][i]->Initialize(
			    darumaYellowModel_, Vector3{0.0f, i * 5.0f - 10.0f, -30.0f}, DarumaType::YELLOW);
			i++;
		}
	}
}

void GameScene::UpdateEnemyPopCommands() {
	// 待機処理
	if (isDefeat_) {
		uint32_t enemyCount = 0;
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
			//// X座標
			// getline(line_stream, word, ',');
			// float x = (float)atof(word.c_str());
			//// Y座標
			// getline(line_stream, word, ',');
			// float y = (float)atof(word.c_str());
			//// Z座標
			// getline(line_stream, word, ',');
			// float z = (float)atof(word.c_str());

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
			// AddEnemy(enemyType, {x, y, z});
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

void GameScene::StageUpdate() {
	// 1フレーム前の達磨カウントを保存
	preDarumaCount_[darumaNum_] = darumaCount_[darumaNum_];
	darumaCount_[darumaNum_] = 0;

	for (uint32_t j = 0; j < kMaxDaruma_; j++) {
		darumaCount_[darumaNum_] += daruma_[darumaNum_][j]->GetIsBreak();
	}
	if (darumaCount_[darumaNum_] >= kMaxDaruma_ && darumaNum_ < kMaxDarumaNum_ - 1) {
		darumaNum_++;
		for (uint32_t i = 0; i < kMaxDaruma_; i++) {
			daruma_[darumaNum_][i]->SetEaseStartPos(daruma_[darumaNum_][i]->GetWorldPosition());
			daruma_[darumaNum_][i]->SetMovePos(startDarumaPos[i]);
		}
	}

	// 1フレーム前の達磨カウントと現在の達磨カウントが違うときに、イージングの初期値と終了値を設定する
	if (darumaCount_[darumaNum_] < kMaxDaruma_ &&
	    darumaCount_[darumaNum_] != preDarumaCount_[darumaNum_]) {
		for (uint32_t j = 1; j < kMaxDaruma_; j++) {
			daruma_[darumaNum_][j]->SetMovePos(daruma_[darumaNum_][j - 1]->GetWorldPosition());
			daruma_[darumaNum_][j]->SetEaseStartPos(daruma_[darumaNum_][j]->GetWorldPosition());
		}
	}
	// 達磨カウントの場所の更新処理を呼ぶ
	// 達磨をイージングさせる
	if (darumaCount_[darumaNum_] < kMaxDaruma_) {
		daruma_[darumaNum_][darumaCount_[darumaNum_]]->Update();
		for (uint32_t j = 0; j < kMaxDaruma_; j++) {
			daruma_[darumaNum_][j]->Move();
		}
	}
}