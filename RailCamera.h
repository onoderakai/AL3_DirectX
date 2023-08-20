#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Input.h"

class RailCamera {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="trans">ワールド座標</param>
	/// <param name="rot">回転</param>
	void Initialize(Vector3 trans, Vector3 rot);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// ビュープロジェクションのゲッター
	/// </summary>
	/// <returns></returns>
	const ViewProjection& GetViewProjection() { return viewProjection_; }

	/// <summary>
	/// ワールドトランスフォームのゲッター
	/// </summary>
	/// <returns></returns>
	const WorldTransform& GetWorldTransform() { return world_; }

private:
	Input* input_;
	WorldTransform world_;
	ViewProjection viewProjection_;
};
