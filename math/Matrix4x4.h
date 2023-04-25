#pragma once
#include "Vector3.h"

/// <summary>
/// 4x4行列
/// </summary>
struct Matrix4x4 final {
	float m[4][4];
};

/// <summary>
/// スケーリング行列
/// </summary>
/// <param name="scale">スケール</param>
/// <returns></returns>
Matrix4x4 MakeScaleMatrix(const Vector3& scale);

/// <summary>
/// 回転行列
/// </summary>
/// <param name="theta"></param>
/// <returns></returns>
Matrix4x4 MakeRotationXMatrix(float theta);

/// <summary>
/// 回転行列
/// </summary>
/// <param name="theta"></param>
/// <returns></returns>
Matrix4x4 MakeRotationYMatrix(float theta);

/// <summary>
/// 回転行列
/// </summary>
/// <param name="theta"></param>
/// <returns></returns>
Matrix4x4 MakeRotationZMatrix(float theta);

/// <summary>
/// 平行移動行列
/// </summary>
/// <param name="trans"></param>
/// <returns></returns>
Matrix4x4 MakeTranslateMatrix(Vector3 trans);

/// <summary>/// 代入演算子オーバーロード/// </summary>/// <param name="m1"></param>/// <param name="m2"></param>/// <returns></returns>
Matrix4x4& operator*=(Matrix4x4& m1, const Matrix4x4& m2);

/// <summary>/// 2公演算子/// </summary>/// <param name="m1"></param>/// <param name="m2"></param>/// <returns></returns>
Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2);


Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& trans);