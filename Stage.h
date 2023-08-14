#pragma once
#include "Input.h"
#include "Scene.h"
#include <list>

#include "Enemy.h"
#include "Player.h"
#include "RailCamera.h"
#include "Skydome.h"
#include "ViewProjection.h"
#include "ParticleSystem.h"


class Stage {

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(SceneNum* pScene);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(ViewProjection& view);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& view);

	/// <summary>
	/// 必要なクラスのセッター
	/// </summary>
	/// <param name="player"></param>
	/// <param name="skydome"></param>
	/// <param name="railCamera"></param>
	void SetParameter(
	    Player* player, Skydome* skydome, RailCamera* railCamera, ParticleSystem* particleSystem);

private:
	Input* input_ = nullptr;
	// シーン変数のポインタ
	SceneNum* pScene_;

	// ビュープロジェクション
	ViewProjection viewProjection_;

	// プレイヤーを包含
	Player* player_ = nullptr;

	// エネミーを包含
	std::list<Enemy*> enemys_;
	// 弾の包含
	std::list<EnemyBullet*> enemyBullets_;

	// 天球を包含
	Skydome* skydome_ = nullptr;

	// レールカメラの包含
	RailCamera* railCamera_ = nullptr;

	// パーティクルシステムの包含
	ParticleSystem* particleSystem_ = nullptr;
};
