#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"
#include <cstdint>

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
	/// 衝突属性のゲッター
	/// </summary>
	/// <returns></returns>
	uint32_t GetCollisionAttribute() { return collisionAttribute_; }

	/// <summary>
	/// 衝突属性のセッター
	/// </summary>
	/// <param name="collisionAttribute"></param>
	void SetCollisonAttribute(uint32_t collisionAttribute) { collisionAttribute_ = collisionAttribute; }

	/// <summary>
	/// 衝突マスクのゲッター
	/// </summary>
	/// <returns></returns>
	uint32_t GetCollisionMask() { return collisionMask_; }

	/// <summary>
	/// 衝突属性のセッター
	/// </summary>
	/// <param name="collisionMask"></param>
	void SetCollisonMask(uint32_t collisionMask) { collisionMask_ = collisionMask; }


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
	//衝突属性
	uint32_t collisionAttribute_ = 0xffffffff;
	//衝突マスク
	uint32_t collisionMask_ = 0xffffffff;
};
