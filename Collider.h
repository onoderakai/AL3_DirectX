#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"
#include "WorldTransform.h"

/// <summary>
/// 衝突判定
/// </summary>
class Collider {
public:

	/// <summary>
	/// 半径のゲッター
	/// </summary>
	/// <returns></returns>
	float GetRadius() { return radius_; }

	/// <summary>
	/// 半径のセッター
	/// </summary>
	/// <param name="radius"></param>
	void SetRadius(float radius) { radius_ = radius; }

	/// <summary>
	/// 衝突判定コールバック
	/// </summary>
	/// <returns></returns>
	virtual void OnCollision();

	/// <summary>
	/// ワールド座標のゲッター
	/// </summary>
	/// <returns></returns>
	virtual Vector3 GetWorldPosition();

private:
	float radius_ = 1.0f;
};
