#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"


/// <summary>
/// ベクトル変換
/// </summary>
/// <param name="v">変換するベクトル</param>
/// <param name="m">基準となる4x4行列</param>
/// <returns></returns>
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

Vector3 Transform(Vector3 Vector, Matrix4x4 Matrix);