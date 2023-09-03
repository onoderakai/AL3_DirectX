﻿#include "RailCamera.h"
#include "ImGuiManager.h"
#include "MathUtility.h"

RailCamera::RailCamera() { shake_ = new Shake(); }

RailCamera::~RailCamera() { delete shake_; }

void RailCamera::Initialize(Vector3 trans, Vector3 rot) {
	input_ = Input::GetInstance();
	world_.Initialize();
	world_.translation_ = trans;
	world_.rotation_ = rot;

	viewProjection_.Initialize();
}

void RailCamera::Update() {
	if (input_->TriggerKey(DIK_C)) {
		shake_->SetShaking(true, 300, Vector2{50.0f, 50.0f}, world_.translation_);
	}
	
	Vector3 forward = {0.0f, 0.0f, 1.0f};
	float speed = 0.1f;
	// ワールド行列の反映
	forward = TransformNormal(forward, world_.matWorld_);
	// 長さを整える
	forward = Normalize(forward) * speed;
	//world_.translation_ += forward;
	
	// 移動処理
	Vector3 move = {};
	const float kCharacterSpeed = 0.6f;
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
	float y = move.y;
	// ワールド行列の反映
	move = TransformNormal(move, world_.matWorld_);
	// 長さを整える
	move = Normalize(move) * kCharacterSpeed;
	// 移動
	move.y = y;
	//world_.translation_ += move;

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
	//world_.rotation_ += rotate;

	world_.translation_ = shake_->EasingShaking(world_);

	world_.UpdateMatrix();
	viewProjection_.matView = Inverse(world_.matWorld_);
	viewProjection_.TransferMatrix();
}
