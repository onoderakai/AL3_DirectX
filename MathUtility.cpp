#include "MathUtility.h"
#include <cassert>
#include <cmath>

Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m) {
	Vector3 result = {};

	result = {
	    v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
	    v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
	    v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2]};

	return result;
}

Vector3 Transform(Vector3 Vector, Matrix4x4 Matrix) {
	Vector3 result = {0}; // w=1がデカルト座標系であるので(x,y,1)のベクトルとしてmatrixとの積をとる

	result.x = Vector.x * Matrix.m[0][0] + Vector.y * Matrix.m[1][0] + Vector.z * Matrix.m[2][0] +
	           1.0f * Matrix.m[3][0];
	result.y = Vector.x * Matrix.m[0][1] + Vector.y * Matrix.m[1][1] + Vector.z * Matrix.m[2][1] +
	           1.0f * Matrix.m[3][1];
	result.z = Vector.x * Matrix.m[0][2] + Vector.y * Matrix.m[1][2] + Vector.z * Matrix.m[2][2] +
	           1.0f * Matrix.m[3][2];
	float w = Vector.x * Matrix.m[0][3] + Vector.y * Matrix.m[1][3] + Vector.z * Matrix.m[2][3] +
	          1.0f * Matrix.m[3][3];

	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;

	return result;
}

Vector3 FaceToDirection(const Vector3& direction) {
	// 行列計算で角度を変える
	/*Vector3 result = {};
	Vector3 theta = Normalize(direction);
	result.y = std::atan2f(theta.x, theta.z);
	Matrix4x4 rotY = MakeRotationYMatrix(-result.y);
	Vector3 velocityZ = TransformNormal(direction, rotY);
	result.x = std::atan2f(-velocityZ.y, velocityZ.z);*/

	// 行列計算をしないで回転
	Vector3 result = {};
	Vector3 theta = Normalize(direction);
	result.y = std::atan2f(theta.x, theta.z);
	float width = sqrtf(theta.x * theta.x + theta.z * theta.z);
	result.x = std::atan2f(-theta.y, width);
	return result;
}

float Length(const Vector2& v) { return sqrtf(v.x * v.x + v.y * v.y); }

float Length(const Vector3& v) { return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z); }