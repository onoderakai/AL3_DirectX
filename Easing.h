#pragma once
#include "Vector3.h"
#include "Vector2.h"
#include <cstdint>
#define PIf 3.1415926f

class Easing {
public:
	/// <summary>
	/// コンスタントイージング
	/// </summary>
	/// <param name="value">イージングさせたい</param>
	/// <param name="start"></param>
	/// <param name="end"></param>
	/// <param name="maxFrame">終了するまでのフレーム数</param>
	/// <param name="isEase">イージングするかどうか</param>
	/// <returns></returns>
	Vector3 ConstantEase(
	    const Vector3& value, const Vector3& start, const Vector3& end, const uint32_t& maxFrame,
	    bool& isEase);

	/// <summary>
	/// イーズイン
	/// </summary>
	/// <param name="value">イージングさせたい</param>
	/// <param name="start"></param>
	/// <param name="end"></param>
	/// <param name="maxFrame">終了するまでのフレーム数</param>
	/// <param name="isEase">イージングするかどうか</param>
	/// <returns></returns>
	Vector3 EaseInSine(
	    const Vector3& value, const Vector3& start, const Vector3& end, const uint32_t& maxFrame,
	    bool& isEase);

	/// <summary>
	/// イーズアウト
	/// </summary>
	/// <param name="value">イージングさせたい</param>
	/// <param name="start"></param>
	/// <param name="end"></param>
	/// <param name="maxFrame">終了するまでのフレーム数</param>
	/// <param name="isEase">イージングするかどうか</param>
	/// <returns></returns>
	Vector3 EaseOutSine(
	    const Vector3& value, const Vector3& start, const Vector3& end, const uint32_t& maxFrame,
	    bool& isEase);

	/// <summary>
	/// イーズアウト
	/// </summary>
	/// <param name="value">イージングさせたい</param>
	/// <param name="start"></param>
	/// <param name="end"></param>
	/// <param name="maxFrame">終了するまでのフレーム数</param>
	/// <param name="isEase">イージングするかどうか</param>
	/// <returns></returns>
	Vector2 EaseOutSine(
	    const Vector2& value, const Vector2& start, const Vector2& end, const uint32_t& maxFrame,
	    bool& isEase);

	/// <summary>
	/// イーズアウト（弾性）
	/// </summary>
	/// <param name="value">イージングさせたい</param>
	/// <param name="start"></param>
	/// <param name="end"></param>
	/// <param name="maxFrame">終了するまでのフレーム数</param>
	/// <param name="isEase">イージングするかどうか</param>
	/// <returns></returns>
	Vector3 EaseOutElastic(
	    const Vector3& value, const Vector3& start, const Vector3& end, const uint32_t& maxFrame,
	    bool& isEase);

	/// <summary>
	/// タイムカウントをリセットする
	/// </summary>
	void ResetTimeCount() { timeCount_ = 0; }

private:
	uint32_t timeCount_ = 0;
};
