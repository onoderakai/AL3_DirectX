#include "Stage.h"
#include "SceneChange.h"
#include "Enemy.h"
#include "EnemyBullet.h"


void Stage::Initialize(SceneNum* pScene) {
	pScene_ = pScene;
	input_ = Input::GetInstance();

	viewProjection_.farZ = 10.0f;
	viewProjection_.Initialize();
}

void Stage::Update(ViewProjection& view) {
	// RETURNでTITLEシーンに遷移する
	if (input_->PushKey(DIK_RETURN)) {
		SceneChange::GetInstance()->Change(SceneNum::TITLE, pScene_);
	}

	railCamera_->Update();
	/*if (!isDebugCamera) {
	}*/
	view.matView = railCamera_->GetViewProjection().matView;
	view.matProjection = railCamera_->GetViewProjection().matProjection;
	view.TransferMatrix();

	// プレイヤーの更新処理
	if (player_) {
		player_->Update(view);
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

	// パーティクルシステムの更新処理
	particleSystem_->Update();
}

void Stage::Draw(ViewProjection& view) {

	if (player_) {
		player_->Draw(view);
	}
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Draw(view);
	}
	for (Enemy* enemy : enemys_) {
		if (enemy) {
			enemy->Draw(view);
		}
	}
	if (skydome_) {
		skydome_->Draw(view);
	}

	particleSystem_->Draw(view);
}

void Stage::SetParameter(
    Player* player, Skydome* skydome, RailCamera* railCamera, ParticleSystem* particleSystem) {
	player_ = player;
	railCamera_ = railCamera;
	skydome_ = skydome;
	particleSystem_ = particleSystem;
}
