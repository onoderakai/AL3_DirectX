#include "Player.h"
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

	uint32_t textureReticle = TextureManager::Load("target.png");
	sprite2DReticle_ = Sprite::Create(textureReticle, {640, 360}, {1, 1, 1, 1}, {0.5f, 0.5f});
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

	// 攻撃処理
	Attack();
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	// 当たり判定
	float moveLimitX = 34.0f;
	float moveLimitY = 18.0f;

	world_.translation_.x = max(world_.translation_.x, -moveLimitX);
	world_.translation_.x = min(world_.translation_.x, +moveLimitX);
	world_.translation_.y = max(world_.translation_.y, -moveLimitY);
	world_.translation_.y = min(world_.translation_.y, +moveLimitY);

	Trans3DReticle();
	WorldToScreen2DReticle(viewProjection);

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

void Player::Draw(ViewProjection& viewProjection) {

	model_->Draw(world_, viewProjection, textureHandle_);
	model_->Draw(world3DReticle_, viewProjection);
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Player::OnCollision() {}

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
	if (input_->PushKey(DIK_SPACE) && bulletCoolTime_ <= 0) {
		bulletCoolTime_ = 5;
		// 自キャラの座標
		Vector3 pos = GetWorldPosition();

		PlayerBullet* newBullet = new PlayerBullet();
		Vector3 bulletVelocity = Get3DReticleWorldPosition() - GetWorldPosition();
		// 弾の移動速度
		float bulletSpeed = 1.0f;
		//弾の速度を正規化し速度をかける
		bulletVelocity = Normalize(bulletVelocity) * bulletSpeed;

		newBullet->Initialeze(model_, pos, bulletVelocity);

		bullets_.push_back(newBullet);
	}
}

void Player::Trans3DReticle() {
	// 自機から3Dレティクルの距離
	float disPlayerTo3DReticle = 50.0f;
	//自機から3Dレティクルのオフセット
	Vector3 offset = {0.0f, 0.0f, 1.0f};
	//自機のワールド行列の反映
	offset = TransformNormal(offset, world_.matWorld_);
	//長さを整える
	offset = Normalize(offset) * disPlayerTo3DReticle;

	world3DReticle_.translation_ = GetWorldPosition() + offset;
	world3DReticle_.UpdateMatrix();
}

void Player::WorldToScreen2DReticle(const ViewProjection& viewProjection) {
	Vector3 ReticlePos = Get3DReticleWorldPosition();
	//ビューポート行列
	Matrix4x4 matViewport = MakeViewportMatrix(
		0.0f, 0.0f, float(WinApp::kWindowWidth), float(WinApp::kWindowHeight), 0.0f, 1.0f);

	//ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matViewProjectionViewport =
	    viewProjection.matView * viewProjection.matProjection * matViewport;

	//ワールドからスクリーン
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

	Matrix4x4 matVPV = viewProjection.matView * viewProjection.matProjection * matViewport;

	Matrix4x4 matInverseVPV = Inverse(matVPV);

	//スクリーン座標
	Vector3 posNear = Vector3(float(mousePos.x), float(mousePos.y), 0.0f);
	Vector3 posFar = Vector3(float(mousePos.x), float(mousePos.y), 1.0f);

	//スクリーン座標からワールド座標に変換
	posNear = Transform(posNear, matInverseVPV);
	posFar = Transform(posFar, matInverseVPV);

	//マウスレイの方向
	Vector3 mouseDirection = posFar - posNear;
	mouseDirection = Normalize(mouseDirection);
	//カメラから照準オブジェクトの距離
	const float kDisTestObj = 50.0f;
	world3DReticle_.translation_ = posNear + (mouseDirection * kDisTestObj);
	world3DReticle_.UpdateMatrix();
}

void Player::DrawUI() { sprite2DReticle_->Draw(); }
