#include "Player.h"
#include "CollisionConfig.h"
#include "Enemy.h"
#include "ImGuiManager.h"
#include "MathUtility.h"
#include <cassert>

Player::~Player() {
	delete sprite2DReticle_;
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Player::Initialeze(Model* model, uint32_t textureHandle, const Vector3& pos) {
	(assert(model));
	(assert(textureHandle));
	model_ = model;
	textureHandle_ = textureHandle;
	world_.Initialize();
	input_ = Input::GetInstance();
	world_.translation_ = pos;

	// 3Dレティクルの初期化
	world3DReticle_.Initialize();
	world3DReticle2_.Initialize();
	world3DReticle3_.Initialize();

	uint32_t textureReticle = TextureManager::Load("target.png");
	sprite2DReticle_ = Sprite::Create(textureReticle, {640, 360}, {1, 1, 1, 1}, {0.5f, 0.5f});

	//衝突フィルタリングを設定
	//  このクラスの属性を設定
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

	// ジョイスティックを使う
	XINPUT_STATE joyState = {};
	// 移動処理
	Move(joyState);

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
	float moveLimitX = 34.0f;
	float moveLimitY = 18.0f;

	world_.translation_.x = max(world_.translation_.x, -moveLimitX);
	world_.translation_.x = min(world_.translation_.x, +moveLimitX);
	world_.translation_.y = max(world_.translation_.y, -moveLimitY);
	world_.translation_.y = min(world_.translation_.y, +moveLimitY);

	world_.UpdateMatrix();
#ifdef DEBUG
	// ImGui
	ImGui::Begin("PlayerPos");
	ImGui::Text(
	    "x:%f:y:%f:z:%f", worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z);
	ImGui::SliderFloat3("", &worldTransform_.translation_.x, 0.0f, 10.0f);
	ImGui::End();

#endif // DEBUG
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

	// 旋回処理
	Vector3 rotate = {};
	const float matRotSpeed = 0.02f;
	// 入力
	if (input_->PushKey(DIK_UP)) {
		rotate.x = -matRotSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		rotate.x = matRotSpeed;
	}
	if (input_->PushKey(DIK_RIGHT)) {
		rotate.y = matRotSpeed;
	} else if (input_->PushKey(DIK_LEFT)) {
		rotate.y = -matRotSpeed;
	}
	// 旋回
	world_.rotation_ += rotate;

	world_.rotation_ = FaceToDirection(Get3DReticleWorldPosition() - GetWorldPosition());
}

void Player::Draw(const ViewProjection& viewProjection) {

	model_->Draw(world_, viewProjection, textureHandle_);
	world3DReticle_.scale_.z = 1.0f;
	world3DReticle_.scale_.x = 1.0f;
	world3DReticle_.scale_.y = 1.0f;
	world3DReticle_.rotation_ = FaceToDirection(Get3DReticleWorldPosition() - GetWorldPosition());
	model_->Draw(world3DReticle_, viewProjection);
	model_->Draw(world3DReticle2_, viewProjection);
	model_->Draw(world3DReticle3_, viewProjection);
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

void Player::Attack() {
	if (bulletCoolTime_ > 0) {
		bulletCoolTime_--;
	}
	if (input_->IsPressMouse(0) && bulletCoolTime_ <= 0) {
		bulletCoolTime_ = 5;
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

		// 弾の移動速度
		float bulletSpeed = 10.0f;
		// 弾の速度を正規化し速度をかける
		bulletVelocity = Normalize(bulletVelocity) * bulletSpeed;

		newBullet->Initialeze(model_, pos, bulletVelocity);

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

	// 敵のロックオン処理
	isLockOn = false;
	nearDis = lockOnDis;
	for (Enemy* enemy : enemys_) {
		Vector3 enemyPos = enemy->GetWorldPosition();
		enemyPos = Transform(enemyPos, matViewProjectionViewport);
		// マウスと敵の距離
		float mouseToEnemyDis =
		    Length(Vector2{float(mousePos.x), float(mousePos.y)} - Vector2{enemyPos.x, enemyPos.y});
		// lockOnDisの範囲内かつ、一番近い敵をlockOnPosにする
		if (nearDis >= mouseToEnemyDis) {
			nearDis = mouseToEnemyDis;
			lockOnPos = enemy->GetWorldPosition();
			isLockOn = true;
			sprite2DReticle_->SetPosition(Vector2{enemyPos.x, enemyPos.y});
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
		world3DReticle_.translation_ = lockOnPos;

		world3DReticle2_.translation_ = world3DReticle_.translation_ + (direction * 10.0f);
		world3DReticle3_.translation_ = world3DReticle_.translation_ + (direction * -10.0f);
	} else {
		world3DReticle_.translation_ = posNear + (mouseDirection * kDisTestObj);

		world3DReticle2_.translation_ = world3DReticle_.translation_ + (direction * 10.0f);
		world3DReticle3_.translation_ = world3DReticle_.translation_ + (direction * -10.0f);
	}

	world3DReticle_.UpdateMatrix();
	world3DReticle2_.UpdateMatrix();
	world3DReticle3_.UpdateMatrix();
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
	world3DReticle2_.translation_ = world3DReticle_.translation_;
	world3DReticle3_.translation_ = world3DReticle_.translation_;

	world3DReticle2_.translation_ = world3DReticle2_.translation_ + (direction * 10.0f);
	world3DReticle3_.translation_ = world3DReticle3_.translation_ + (direction * -10.0f);

	world3DReticle_.UpdateMatrix();
	world3DReticle2_.UpdateMatrix();
	world3DReticle3_.UpdateMatrix();
}

void Player::DrawUI() { sprite2DReticle_->Draw(); }

void Player::OnCollision() {
	hp_--;
	if (hp_ <= 0) {
		isDead_ = true;
	}
}
