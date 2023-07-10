#pragma once

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
	float x;
	float y;
	float z;
};

/// <summary>
/// ベクトルの正規化
/// </summary>
/// <param name="v"></param>
Vector3 Normalize(const Vector3& v);

/// <summary>
/// 線形補間
/// </summary>
/// <param name="p1"></param>
/// <param name="p2"></param>
/// <param name="t"></param>
/// <returns></returns>
Vector3 Lerp(const Vector3& p1, const Vector3& p2, float t);

/// <summary>
/// 球面補間
/// </summary>
/// <param name="p1"></param>
/// <param name="p2"></param>
/// <param name="t"></param>
/// <returns></returns>
Vector3 Slerp(const Vector3& p1, const Vector3& p2, float t);

/// <summary>
/// ベクトル同士の足し算
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
Vector3& operator+=(Vector3& v1, const Vector3& v2);

/// <summary>
/// ベクトル同士の足し算
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
Vector3 operator+(const Vector3& v1, const Vector3& v2);

/// <summary>
/// ベクトル同士の引き算
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
Vector3& operator-=(Vector3& v1, const Vector3& v2);

/// <summary>
/// ベクトル同士の引き算
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
Vector3 operator-(const Vector3& v1, const Vector3& v2);

/// <summary>
/// ベクトルとfloatの掛け算
/// </summary>
/// <param name="v"></param>
/// <param name="value"></param>
/// <returns></returns>
Vector3 operator*(const Vector3& v, const float& value);