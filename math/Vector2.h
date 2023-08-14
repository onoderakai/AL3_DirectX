#pragma once

/// <summary>
/// 2次元ベクトル
/// </summary>
struct Vector2 final {
	float x;
	float y;
};

/// <summary>
/// ベクトル同士の足し算
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
Vector2& operator+=(Vector2& v1, const Vector2& v2);

/// <summary>
/// ベクトル同士の足し算
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
Vector2 operator+(const Vector2& v1, const Vector2& v2);

/// <summary>
/// ベクトル同士の引き算
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
Vector2& operator-=(Vector2& v1, const Vector2& v2);

/// <summary>
/// ベクトル同士の引き算
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
Vector2 operator-(const Vector2& v1, const Vector2& v2);

/// <summary>
/// ベクトルとfloatの掛け算
/// </summary>
/// <param name="v"></param>
/// <param name="value"></param>
/// <returns></returns>
Vector2 operator*(const Vector2& v, const float& value);
