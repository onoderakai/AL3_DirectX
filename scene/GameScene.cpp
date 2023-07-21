#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include <cassert>
#include <cmath>
#include <fstream>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete player_;
	delete playerModel_;
	for (Enemy* enemy : enemys_) {
		delete enemy;
	}
	for (EnemyBullet* bullet : enemyBullets_) {
		delete bullet;
	}
	delete enemyModel_;
	delete skydomeModel_;
	delete skydome_;
	delete railCamera_;

	delete debugCamera_;
}

void GameScene::Initialize() {

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// プレイヤーモデルの生成
	playerModel_ = Model::Create();
	// プレイヤーのテクスチャハンドルを代入
	playerTextureHandle_ = TextureManager::Load("sample.png");

	// エネミーモデルの生成
	enemyModel_ = Model::Create();

	// 天球のモデルを生成
	skydomeModel_ = Model::CreateFromOBJ("skydome", true);

	// インスタンスの生成
	// プレイヤー
	player_ = new Player();
	Vector3 playerPos = {0.0f, -7.0f, 20.0f};
	player_->Initialeze(playerModel_, playerTextureHandle_, playerPos);

	// エネミー
	enemyGeneratePos_ = {5.0f, 3.0f, 100.0f};
	AddEnemy(enemyGeneratePos_);

	// 天球
	skydome_ = new Skydome();
	skydome_->Initialize(skydomeModel_);

	// レールカメラ
	railCamera_ = new RailCamera();
	railCamera_->Initialize({0.0f, 0.0f, -100.0f}, {0.0f, 0.0f, 0.0f});

	// 親子関係を結ぶ
	player_->SetParent(&railCamera_->GetWorldTransform());

	viewProjection_.farZ = 100.0f;
	viewProjection_.Initialize();

	//ファイル読み込み
	LoadEnemyPopData();

	//レティクル画像読み込み
	TextureManager::Load("target.png");
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
	UpdateEnemyPopCommands();

	railCamera_->Update();
	if (!isDebugCamera) {
		viewProjection_.matView = railCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	}

	// プレイヤーの更新処理
	if (player_) {
		player_->Update(viewProjection_);
	}

	// デスフラグがtrueの敵を削除する
	enemys_.remove_if([](Enemy* enemy) {
		if (enemy->GetIsDead()) {
			delete enemy;
			return true;
		}
		return false;
	});

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

	CheckAllCollision();
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
	if (player_) {
		player_->Draw(viewProjection_);
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
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	player_->DrawUI();
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollision() {
	// 判定対象の座標
	Vector3 posA = {};
	Vector3 posB = {};

	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	const std::list<EnemyBullet*>& enemyBullets = enemyBullets_;

	// 自キャラの座標
	posA = player_->GetWorldPosition();

	// 自キャラと敵弾全ての当たり判定
	for (EnemyBullet* bullet : enemyBullets) {
		//// 敵弾の座標
		//posB = bullet->GetWorldPosition();

		//// 距離を求める
		//float length = Length(posA - posB);
		//// 半径同士の足し算
		//float radius = player_->GetRadius() + bullet->GetRadius();
		//if (length <= radius) {
		//	player_->OnCollision();
		//	bullet->OnCollision();
		//}
		CheckCollisionPair(player_, bullet);
	}

	for (Enemy* enemy : enemys_) {
		// 敵キャラの座標
		posA = enemy->GetWorldPosition();
		// 敵キャラと自弾全ての当たり判定
		for (PlayerBullet* bullet : playerBullets) {
			//// 自弾の座標
			//posB = bullet->GetWorldPosition();

			//// 距離を求める
			//float length = Length(posA - posB);
			//// 半径同士の足し算
			//float radius = enemy->GetRadius() + bullet->GetRadius();
			//if (length <= radius) {
			//	enemy->OnCollision();
			//	bullet->OnCollision();
			//}
			CheckCollisionPair(enemy, bullet);
		}
	}

	// 自弾と敵弾の当たり判定
	for (PlayerBullet* playerBullet : playerBullets) {
		// 自弾の座標
		posA = playerBullet->GetWorldPosition();
		for (EnemyBullet* enemyBullet : enemyBullets) {
			//// 敵弾の座標
			//posB = enemyBullet->GetWorldPosition();
			//// 距離を求める
			//float length = Length(posA - posB);
			//// 半径同士の足し算
			//float radius = playerBullet->GetRadius() + enemyBullet->GetRadius();
			//if (length <= radius) {
			//	playerBullet->OnCollision();
			//	enemyBullet->OnCollision();
			//}
			CheckCollisionPair(playerBullet, enemyBullet);
		}
	}
}

void GameScene::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {
	//座標の保存
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

float GameScene::Length(const Vector3& v) { return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z); }

void GameScene::LoadEnemyPopData() {

	// ファイル読み込み
	ifstream file;
	file.open("./Resources/enemyPop.csv");
	assert(file.is_open());
	// ファイルをコピー
	enemyPopCommands << file.rdbuf();
	// ファイルを閉じる
	file.close();
}

void GameScene::UpdateEnemyPopCommands() {
	//待機処理
	if (isWait_) {
		waitTime_--;
		if (waitTime_ <= 0) {
			isWait_ = false;
		}
		return;
	}

	//1行分の文字列を入れる変数
	string line;

	//コマンド実行ループ
	while (getline(enemyPopCommands, line)) {

		istringstream line_stream(line);

		string word;
		getline(line_stream, word, ',');

		// //から始まる行はコメントのため飛ばす
		if (word.find("//") == 0) {
			// コメント行を飛ばす
			continue;
		}

		if (word.find("POP") == 0) {
			//X座標
			getline(line_stream, word, ',');
			float x = (float)atof(word.c_str());
			// Y座標
			getline(line_stream, word, ',');
			float y = (float)atof(word.c_str());
			// Z座標
			getline(line_stream, word, ',');
			float z = (float)atof(word.c_str());
			AddEnemy({x, y, z});
		}
		else if (word.find("WAIT") == 0) {
		
			getline(line_stream, word, ',');

			int32_t waitTime = atoi(word.c_str());

			isWait_ = true;
			waitTime_ = waitTime;

			//コマンドループを抜ける
			break;
		}
	}
}

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {
	// リストに追加する
	enemyBullets_.push_back(enemyBullet);
}

void GameScene::AddEnemy(Vector3 pos) {
	Enemy* newEnemy = new Enemy();
	newEnemy->SetPlayer(player_);
	newEnemy->SetGameScene(this);
	newEnemy->Initialeze(enemyModel_, pos);
	enemys_.push_back(newEnemy);
}
