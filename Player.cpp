#include "Player.h"
#include "CollisionConfig.h"
#include "Enemy.h"
#include "ImGuiManager.h"
#include "MathUtility.h"
#include "ParticleSystem.h"
#include "Boss.h"
#include <cassert>

Player::Player() {
	uint32_t textureReticle = TextureManager::Load("target.png");
	uint32_t hpTexture = TextureManager::Load("player_hp.png");
	sprite2DReticle_ = Sprite::Create(textureReticle, {640, 360}, {1, 1, 1, 1}, {0.5f, 0.5f});
	sprite2DReticleBack_ = Sprite::Create(textureReticle, {640, 360}, {1, 1, 1, 1}, {0.5f, 0.5f});
	sprite2DReticleBack_->SetSize(Vector2{40.0f, 40.0f});
	sprite2DReticleFront_ = Sprite::Create(textureReticle, {640, 360}, {1, 1, 1, 1}, {0.5f, 0.5f});
	sprite2DReticleFront_->SetSize(Vector2{80.0f, 80.0f});
	bulletModel_ = Model::CreateFromOBJ("PlayerBullet", true);
	for (uint32_t i = 0; i < kMaxHp_; i++) {
		hpSprite_[i] =
		    Sprite::Create(hpTexture, {48 + (48.0f * i), 688}, {1, 1, 1, 1}, {0.5f, 0.5f});
	}
}

Player::~Player() {
	delete sprite2DReticle_;
	delete sprite2DReticleFront_;
	delete sprite2DReticleBack_;
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
	delete bulletModel_;
	for (uint32_t i = 0; i < kMaxHp_; i++) {
		delete hpSprite_[i];
	}
}

void Player::Initialeze(Model* model, Model* sniperModel, const Vector3& pos) {
	bullets_.remove_if([](PlayerBullet* bullet) {
		delete bullet;
		return true;
	});

	(assert(model));
	model_ = model;
	sniperModel_ = sniperModel;
	world_.Initialize();
	input_ = Input::GetInstance();
	world_.translation_ = pos;
	world_.scale_ = {0.5f, 0.5f, 0.5f};

	style_ = Style::NORMAL;
	isDead_ = false;
	hp_ = kMaxHp_;
	bulletCoolTime_ = 0;

	// 3Dレティクルの初期化
	world3DReticle_.Initialize();
	world3DReticleBack_.Initialize();
	world3DReticleFront_.Initialize();

	// 衝突フィルタリングを設定
	//   このクラスの属性を設定
	SetCollisonAttribute(kCollisionAttributePlayer);
	// このクラスの衝突しない属性を設定
	SetCollisonMask(GetCollisionMask() ^ kCollisionAttributePlayer);
}

void Player::Update(const ViewProjection& viewProjection) {

	// デスフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->GetIsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	StyleChange();

	// ジョイスティックを使う
	XINPUT_STATE joyState = {};
	// 移動処理
	switch (style_) {
	case Player::Style::NORMAL:
		Move(joyState);
		break;
	case Player::Style::SNIPER:
		break;
	default:
		break;
	}
	world_.rotation_ = FaceToDirection(Get3DReticleWorldPosition() - GetWorldPosition());

	// 攻撃処理
	Attack();
	// 弾の更新処理
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	// Trans3DReticle();
	WorldToScreen2DReticle(viewProjection);
	// ScreenToWorld2DReticle(viewProjection, joyState);

	// 当たり判定
	float moveLimitX = 12.0f;
	float moveLimitY = 7.0f;

	world_.translation_.x = max(world_.translation_.x, -moveLimitX);
	world_.translation_.x = min(world_.translation_.x, +moveLimitX);
	world_.translation_.y = max(world_.translation_.y, -moveLimitY);
	world_.translation_.y = min(world_.translation_.y, +moveLimitY);

	world_.UpdateMatrix();
#ifdef _DEBUG
	ImGui::Begin("PlayerHp");
	ImGui::Text("%d", hp_);
	ImGui::End();
#endif // _DEBUG
}

void Player::Move(XINPUT_STATE& joyState) {
	// 移動処理
	Vector3 move = {};
	const float kCharacterSpeed = 0.2f;
	// 入力
	if (input_->PushKey(DIK_A)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_D)) {
		move.x += kCharacterSpeed;
	}
	if (input_->PushKey(DIK_W)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_S)) {
		move.y -= kCharacterSpeed;
	}
	// ジョイスティックで移動
	if (input_->GetInstance()->GetJoystickState(0, joyState)) {
		move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed;
		move.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed;
	}
	// 移動
	world_.translation_ += move;

	world_.rotation_ = FaceToDirection(Get3DReticleWorldPosition() - GetWorldPosition());
}

void Player::Draw(const ViewProjection& viewProjection) {
	switch (style_) {
	case Player::Style::NORMAL:
		model_->Draw(world_, viewProjection);
		break;
	case Player::Style::SNIPER:
		sniperModel_->Draw(world_, viewProjection);
		break;
	default:
		break;
	}
	world3DReticle_.rotation_ = FaceToDirection(Get3DReticleWorldPosition() - GetWorldPosition());
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

Vector3 Player::GetWorldPosition() {
	Vector3 worldPos = {};
	worldPos = {world_.matWorld_.m[3][0], world_.matWorld_.m[3][1], world_.matWorld_.m[3][2]};
	return worldPos;
}

Vector3 Player::Get3DReticleWorldPosition() {
	Vector3 worldPos = {};
	worldPos = {
	    world3DReticle_.matWorld_.m[3][0], world3DReticle_.matWorld_.m[3][1],
	    world3DReticle_.matWorld_.m[3][2]};
	return worldPos;
}

Vector3 Player::Get3DReticleFrontWorldPosition() {
	Vector3 worldPos = {};
	worldPos = {
	    world3DReticleFront_.matWorld_.m[3][0], world3DReticleFront_.matWorld_.m[3][1],
	    world3DReticleFront_.matWorld_.m[3][2]};
	return worldPos;
}

Vector3 Player::Get3DReticleBackWorldPosition() {
	Vector3 worldPos = {};
	worldPos = {
	    world3DReticleBack_.matWorld_.m[3][0], world3DReticleBack_.matWorld_.m[3][1],
	    world3DReticleBack_.matWorld_.m[3][2]};
	return worldPos;
}

void Player::StyleChange() {
	if (input_->TriggerKey(DIK_R)) {
		switch (style_) {
		case Player::Style::NORMAL:
			style_ = Style::SNIPER;
			break;
		case Player::Style::SNIPER:
			style_ = Style::NORMAL;
			break;
		default:
			break;
		}
	}
}

void Player::Attack() {
	if (bulletCoolTime_ > 0) {
		bulletCoolTime_--;
	}
	if (input_->IsPressMouse(0) && bulletCoolTime_ <= 0) {
		// 弾の移動速度
		float bulletSpeed = 1.0f;
		switch (style_) {
		case Player::Style::NORMAL:
			bulletCoolTime_ = kNormalCoolTime_;
			bulletSpeed = kNormalBulletSpeed_;
			break;
		case Player::Style::SNIPER:
			bulletCoolTime_ = kSniperCoolTime_;
			if (isLockOn) {
				isLockOn = false;
				lockOnCollider_->OnCollision();
			}
			bulletSpeed = kSniperBulletSpeed_;
			break;
		default:
			break;
		}

		// 自キャラの座標
		Vector3 pos = GetWorldPosition();

		PlayerBullet* newBullet = new PlayerBullet();
		// 弾の速度
		Vector3 bulletVelocity = {0.0f, 0.0f, 1.0f};

		if (isLockOn) {
			bulletVelocity = lockOnPos - GetWorldPosition();
		} else {
			bulletVelocity = Get3DReticleWorldPosition() - GetWorldPosition();
		}

		// 弾の速度を正規化し速度をかける
		bulletVelocity = Normalize(bulletVelocity) * bulletSpeed;

		newBullet->Initialeze(bulletModel_, pos, bulletVelocity);

		bullets_.push_back(newBullet);
	}
}

void Player::Trans3DReticle() {
	// 自機から3Dレティクルの距離
	float disPlayerTo3DReticle = 50.0f;
	// 自機から3Dレティクルのオフセット
	Vector3 offset = {0.0f, 0.0f, 1.0f};
	// 自機のワールド行列の反映
	offset = TransformNormal(offset, world_.matWorld_);
	// 長さを整える
	offset = Normalize(offset) * disPlayerTo3DReticle;

	world3DReticle_.translation_ = GetWorldPosition() + offset;
	world3DReticle_.UpdateMatrix();
}

void Player::WorldToScreen2DReticle(const ViewProjection& viewProjection) {
	Vector3 ReticlePos = Get3DReticleWorldPosition();
	// ビューポート行列
	Matrix4x4 matViewport = MakeViewportMatrix(
	    0.0f, 0.0f, float(WinApp::kWindowWidth), float(WinApp::kWindowHeight), 0.0f, 1.0f);

	// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matViewProjectionViewport =
	    viewProjection.matView * viewProjection.matProjection * matViewport;

	// ワールドからスクリーン
	ReticlePos = Transform(ReticlePos, matViewProjectionViewport);

	sprite2DReticle_->SetPosition(Vector2{ReticlePos.x, ReticlePos.y});

	///////マウス座標に変換/////////
	// マウス座標
	POINT mousePos = {};
	GetCursorPos(&mousePos);
	// クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePos);

	sprite2DReticle_->SetPosition(Vector2{float(mousePos.x), float(mousePos.y)});

	// ロックオン処理
	isLockOn = false;
	if (style_ == Style::SNIPER) {
		nearDis = lockOnDis;
		if (boss_) {
			Vector3 bossPos = boss_->GetWorldPosition();
			bossPos = Transform(bossPos, matViewProjectionViewport);
			// マウスと敵の距離
			float mouseToEnemyDis = Length(
			    Vector2{float(mousePos.x), float(mousePos.y)} - Vector2{bossPos.x, bossPos.y});
			// lockOnDisの範囲内かつ、一番近い敵をlockOnPosにする
			if (nearDis >= mouseToEnemyDis) {
				nearDis = mouseToEnemyDis;
				lockOnPos = boss_->GetWorldPosition();
				lockOnCollider_ = boss_;
				isLockOn = true;
				sprite2DReticle_->SetPosition(Vector2{bossPos.x, bossPos.y});
			}
		}
		for (Enemy* enemy : enemys_) {
			Vector3 enemyPos = enemy->GetWorldPosition();
			enemyPos = Transform(enemyPos, matViewProjectionViewport);
			// マウスと敵の距離
			float mouseToEnemyDis = Length(
			    Vector2{float(mousePos.x), float(mousePos.y)} - Vector2{enemyPos.x, enemyPos.y});
			// lockOnDisの範囲内かつ、一番近い敵をlockOnPosにする
			if (nearDis >= mouseToEnemyDis) {
				nearDis = mouseToEnemyDis;
				lockOnPos = enemy->GetWorldPosition();
				lockOnCollider_ = enemy;
				isLockOn = true;
				sprite2DReticle_->SetPosition(Vector2{enemyPos.x, enemyPos.y});
			}
		}
	}

	Matrix4x4 matVPV = viewProjection.matView * viewProjection.matProjection * matViewport;

	Matrix4x4 matInverseVPV = Inverse(matVPV);

	// スクリーン座標
	Vector3 posNear = Vector3(float(mousePos.x), float(mousePos.y), 0.0f);
	Vector3 posFar = Vector3(float(mousePos.x), float(mousePos.y), 1.0f);

	// スクリーン座標からワールド座標に変換
	posNear = Transform(posNear, matInverseVPV);
	posFar = Transform(posFar, matInverseVPV);

	// マウスレイの方向
	Vector3 mouseDirection = posFar - posNear;
	mouseDirection = Normalize(mouseDirection);
	// カメラから照準オブジェクトの距離
	const float kDisTestObj = 50.0f;
	// プレイヤーと3Dレティクルのベクトル
	Vector3 direction = Get3DReticleWorldPosition() - GetWorldPosition();
	direction = Normalize(direction);
	if (isLockOn) {
		sprite2DReticleBack_->SetRotation(sprite2DReticleBack_->GetRotation() + 0.1f);
		sprite2DReticleFront_->SetRotation(sprite2DReticleFront_->GetRotation() - 0.1f);

		world3DReticle_.translation_ = lockOnPos;
		world3DReticleBack_.translation_ = world3DReticle_.translation_ + (direction * 10.0f);
		world3DReticleFront_.translation_ = world3DReticle_.translation_ + (direction * -10.0f);
	} else {
		sprite2DReticleBack_->SetRotation(0.0f);
		sprite2DReticleFront_->SetRotation(0.0f);

		world3DReticle_.translation_ = posNear + (mouseDirection * kDisTestObj);
		world3DReticleBack_.translation_ = world3DReticle_.translation_ + (direction * 10.0f);
		world3DReticleFront_.translation_ = world3DReticle_.translation_ + (direction * -10.0f);
	}

	ReticlePos = Get3DReticleFrontWorldPosition();
	// ワールドからスクリーン
	ReticlePos = Transform(ReticlePos, matViewProjectionViewport);
	sprite2DReticleFront_->SetPosition(Vector2{ReticlePos.x, ReticlePos.y});

	ReticlePos = Get3DReticleBackWorldPosition();
	// ワールドからスクリーン
	ReticlePos = Transform(ReticlePos, matViewProjectionViewport);
	sprite2DReticleBack_->SetPosition(Vector2{ReticlePos.x, ReticlePos.y});

	world3DReticle_.UpdateMatrix();
	world3DReticleBack_.UpdateMatrix();
	world3DReticleFront_.UpdateMatrix();
}

void Player::ScreenToWorld2DReticle(const ViewProjection& viewProjection, XINPUT_STATE& joyState) {
	// ジョイスティックかキーボードで2Dスプライトを移動させる
	Vector2 spritePos = sprite2DReticle_->GetPosition();
	const float kSpriteMoveSpeed = 5.0f;
	if (input_->GetInstance()->GetJoystickState(0, joyState)) {
		spritePos.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * kSpriteMoveSpeed;
		spritePos.y += (float)joyState.Gamepad.sThumbRY / SHRT_MAX * kSpriteMoveSpeed;

		sprite2DReticle_->SetPosition(spritePos);
	}
	if (input_->PushKey(DIK_LEFT)) {
		spritePos.x -= kSpriteMoveSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		spritePos.x += kSpriteMoveSpeed;
	}
	if (input_->PushKey(DIK_UP)) {
		spritePos.y -= kSpriteMoveSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		spritePos.y += kSpriteMoveSpeed;
	}
	sprite2DReticle_->SetPosition(spritePos);
	// スクリーン座標
	Vector3 posNear = Vector3(float(spritePos.x), float(spritePos.y), 0.0f);
	Vector3 posFar = Vector3(float(spritePos.x), float(spritePos.y), 1.0f);

	// ビューポート行列
	Matrix4x4 matViewport = MakeViewportMatrix(
	    0.0f, 0.0f, float(WinApp::kWindowWidth), float(WinApp::kWindowHeight), 0.0f, 1.0f);

	Matrix4x4 matVPV = viewProjection.matView * viewProjection.matProjection * matViewport;
	Matrix4x4 matInverseVPV = Inverse(matVPV);
	// スクリーン座標からワールド座標に変換
	posNear = Transform(posNear, matInverseVPV);
	posFar = Transform(posFar, matInverseVPV);

	// レイの方向
	Vector3 rayDirection = posFar - posNear;
	rayDirection = Normalize(rayDirection);
	// カメラから照準オブジェクトの距離
	const float kDisTestObj = 50.0f;
	Vector3 direction = Get3DReticleWorldPosition() - GetWorldPosition();
	direction = Normalize(direction);
	world3DReticle_.translation_ = posNear + (rayDirection * kDisTestObj);
	world3DReticleBack_.translation_ = world3DReticle_.translation_;
	world3DReticleFront_.translation_ = world3DReticle_.translation_;

	world3DReticleBack_.translation_ = world3DReticleBack_.translation_ + (direction * -10.0f);
	world3DReticleFront_.translation_ = world3DReticleFront_.translation_ + (direction * 10.0f);

	world3DReticle_.UpdateMatrix();
	world3DReticleBack_.UpdateMatrix();
	world3DReticleFront_.UpdateMatrix();
}

void Player::HpDraw() {
	for (int i = 0; i < hp_; i++) {
		hpSprite_[i]->Draw();
	}
}

void Player::DrawUI() {
	sprite2DReticle_->Draw();
	sprite2DReticleFront_->Draw();
	sprite2DReticleBack_->Draw();
	HpDraw();
}

void Player::OnCollision() {
	hp_--;
	if (hp_ <= 0) {
		Particle::Parameter para = {};
		para.deathTimer_ = 30;
		para.type_ = Particle::Type::SCALE_CHANGE;
		para.world_.translation_ = GetWorldPosition();
		particleSystem_->Generate(para, 20);
		isDead_ = true;
	}
}
