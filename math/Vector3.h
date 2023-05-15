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
