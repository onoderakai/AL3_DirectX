#include "Matrix4x4.h"
#include <cassert>
#include <cmath>

Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 result = {};

	result = {scale.x, 0.0f, 0.0f,    0.0f, 0.0f, scale.y, 0.0f, 0.0f,
	          0.0f,    0.0f, scale.z, 0.0f, 0.0f, 0.0f,    0.0f, 1.0f};

	return result;
}

Matrix4x4 MakeRotationXMatrix(float theta) {
	float sin = std::sinf(theta);
	float cos = std::cosf(theta);

	Matrix4x4 result = {};

	result = {
	    1.0f, 0.0f, 0.0f, 0.0f, 0.0f, cos, sin, 0.0f, 0.0f, -sin, cos, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	};

	return result;
}

Matrix4x4 MakeRotationYMatrix(float theta) {
	float sin = std::sinf(theta);
	float cos = std::cosf(theta);

	Matrix4x4 result = {};

	result = {
	    cos, 0.0f, -sin, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, sin, 0.0f, cos, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	};

	return result;
}

Matrix4x4 MakeRotationZMatrix(float theta) {
	float sin = std::sinf(theta);
	float cos = std::cosf(theta);

	Matrix4x4 result = {};

	result = {
	    cos, sin, 0.0f, 0.0f, -sin, cos, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	};

	return result;
}

Matrix4x4 MakeTranslateMatrix(Vector3 trans) {
	Matrix4x4 result = {};
	result = {
	    1.0f, 0.0f, 0.0f, 0.0f, 0.0f,    1.0f,    0.0f,    0.0f,
	    0.0f, 0.0f, 1.0f, 0.0f, trans.x, trans.y, trans.z, 1.0f,
	};

	return result;
}

Matrix4x4 MakeViewportMatrix(
    float left, float top, float width, float height, float minDepth, float maxDepth) {
	Matrix4x4 result = {
	    width / 2.0f,
	    0.0f,
	    0.0f,
	    0.0f,
	    0.0f,
	    -height / 2.0f,
	    0.0f,
	    0.0f,
	    0.0f,
	    0.0f,
	    maxDepth - minDepth,
	    0.0f,
	    left + (width / 2.0f),
	    top + (height / 2.0f),
	    minDepth,
	    1.0f};

	return result;
}

Matrix4x4& operator*=(Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result = {};

	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 4; j++) {
			for (size_t k = 0; k < 4; k++) {
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}

	m1 = result;
	return m1;
}

Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result = m1;

	return result *= m2;
}

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& trans) {

	// スケーリング行列
	Matrix4x4 matScale = MakeScaleMatrix(scale);

	// 回転行列
	Matrix4x4 matRotX = MakeRotationXMatrix(rot.x);
	Matrix4x4 matRotY = MakeRotationYMatrix(rot.y);
	Matrix4x4 matRotZ = MakeRotationZMatrix(rot.z);

	// 回転行列の合成(Z * X * Y)
	Matrix4x4 matRot = matRotZ * matRotX * matRotY;

	// 平行移動行列
	Matrix4x4 matTrans = MakeTranslateMatrix(trans);

	// 合成
	Matrix4x4 result = {};
	result = matScale * matRot * matTrans;

	return result;
}

/// <summary>
/// 逆行列4x4
/// </summary>
/// <param name="matrix"></param>
/// <returns></returns>
Matrix4x4 Inverse(const Matrix4x4& matrix) {
	Matrix4x4 result = {0};
	float determinant = matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][3] +
	                    matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][1] +
	                    matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][2] -
	                    matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][1] -
	                    matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][3] -
	                    matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][2] -
	                    matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][3] -
	                    matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][1] -
	                    matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][2] +
	                    matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][1] +
	                    matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][3] +
	                    matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][2] +
	                    matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][3] +
	                    matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][1] +
	                    matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][2] -
	                    matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][1] -
	                    matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][3] -
	                    matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][2] -
	                    matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][0] -
	                    matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][0] -
	                    matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][0] +
	                    matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][0] +
	                    matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][0] +
	                    matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][0];

	assert(determinant != 0.0f);
	float determinantRecp = 1.0f / determinant;
	//
	result.m[0][0] = (matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][3] +
	                  matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][1] +
	                  matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][2] -
	                  matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][1] -
	                  matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][3] -
	                  matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][2]) *
	                 determinantRecp;
	result.m[0][1] = -(matrix.m[0][1] * matrix.m[2][2] * matrix.m[3][3] +
	                   matrix.m[0][2] * matrix.m[2][3] * matrix.m[3][1] +
	                   matrix.m[0][3] * matrix.m[2][1] * matrix.m[3][2] -
	                   matrix.m[0][3] * matrix.m[2][2] * matrix.m[3][1] -
	                   matrix.m[0][2] * matrix.m[2][1] * matrix.m[3][3] -
	                   matrix.m[0][1] * matrix.m[2][3] * matrix.m[3][2]) *
	                 determinantRecp;
	result.m[0][2] = (matrix.m[0][1] * matrix.m[1][2] * matrix.m[3][3] +
	                  matrix.m[0][2] * matrix.m[1][3] * matrix.m[3][1] +
	                  matrix.m[0][3] * matrix.m[1][1] * matrix.m[3][2] -
	                  matrix.m[0][3] * matrix.m[1][2] * matrix.m[3][1] -
	                  matrix.m[0][2] * matrix.m[1][1] * matrix.m[3][3] -
	                  matrix.m[0][1] * matrix.m[1][3] * matrix.m[3][2]) *
	                 determinantRecp;
	result.m[0][3] = -(matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][3] +
	                   matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][1] +
	                   matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][2] -
	                   matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][1] -
	                   matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][3] -
	                   matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][2]) *
	                 determinantRecp;
	//
	result.m[1][0] = -(matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][3] +
	                   matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][0] +
	                   matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][2] -
	                   matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][0] -
	                   matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][3] -
	                   matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][2]) *
	                 determinantRecp;
	result.m[1][1] = (matrix.m[0][0] * matrix.m[2][2] * matrix.m[3][3] +
	                  matrix.m[0][2] * matrix.m[2][3] * matrix.m[3][0] +
	                  matrix.m[0][3] * matrix.m[2][0] * matrix.m[3][2] -
	                  matrix.m[0][3] * matrix.m[2][2] * matrix.m[3][0] -
	                  matrix.m[0][2] * matrix.m[2][0] * matrix.m[3][3] -
	                  matrix.m[0][0] * matrix.m[2][3] * matrix.m[3][2]) *
	                 determinantRecp;
	result.m[1][2] = -(matrix.m[0][0] * matrix.m[1][2] * matrix.m[3][3] +
	                   matrix.m[0][2] * matrix.m[1][3] * matrix.m[3][0] +
	                   matrix.m[0][3] * matrix.m[1][0] * matrix.m[3][2] -
	                   matrix.m[0][3] * matrix.m[1][2] * matrix.m[3][0] -
	                   matrix.m[0][2] * matrix.m[1][0] * matrix.m[3][3] -
	                   matrix.m[0][0] * matrix.m[1][3] * matrix.m[3][2]) *
	                 determinantRecp;
	result.m[1][3] = (matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][3] +
	                  matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][0] +
	                  matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][2] -
	                  matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][0] -
	                  matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][3] -
	                  matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][2]) *
	                 determinantRecp;
	//
	result.m[2][0] = (matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][3] +
	                  matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][0] +
	                  matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][1] -
	                  matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][0] -
	                  matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][3] -
	                  matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][1]) *
	                 determinantRecp;
	result.m[2][1] = -(matrix.m[0][0] * matrix.m[2][1] * matrix.m[3][3] +
	                   matrix.m[0][1] * matrix.m[2][3] * matrix.m[3][0] +
	                   matrix.m[0][3] * matrix.m[2][0] * matrix.m[3][1] -
	                   matrix.m[0][3] * matrix.m[2][1] * matrix.m[3][0] -
	                   matrix.m[0][1] * matrix.m[2][0] * matrix.m[3][3] -
	                   matrix.m[0][0] * matrix.m[2][3] * matrix.m[3][1]) *
	                 determinantRecp;
	result.m[2][2] = (matrix.m[0][0] * matrix.m[1][1] * matrix.m[3][3] +
	                  matrix.m[0][1] * matrix.m[1][3] * matrix.m[3][0] +
	                  matrix.m[0][3] * matrix.m[1][0] * matrix.m[3][1] -
	                  matrix.m[0][3] * matrix.m[1][1] * matrix.m[3][0] -
	                  matrix.m[0][1] * matrix.m[1][0] * matrix.m[3][3] -
	                  matrix.m[0][0] * matrix.m[1][3] * matrix.m[3][1]) *
	                 determinantRecp;
	result.m[2][3] = -(matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][3] +
	                   matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][0] +
	                   matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][1] -
	                   matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][0] -
	                   matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][3] -
	                   matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][1]) *
	                 determinantRecp;
	//
	result.m[3][0] = -(matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][2] +
	                   matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][0] +
	                   matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][1] -
	                   matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][0] -
	                   matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][2] -
	                   matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][1]) *
	                 determinantRecp;
	result.m[3][1] = (matrix.m[0][0] * matrix.m[2][1] * matrix.m[3][2] +
	                  matrix.m[0][1] * matrix.m[2][2] * matrix.m[3][0] +
	                  matrix.m[0][2] * matrix.m[2][0] * matrix.m[3][1] -
	                  matrix.m[0][2] * matrix.m[2][1] * matrix.m[3][0] -
	                  matrix.m[0][1] * matrix.m[2][0] * matrix.m[3][2] -
	                  matrix.m[0][0] * matrix.m[2][2] * matrix.m[3][1]) *
	                 determinantRecp;
	result.m[3][2] = -(matrix.m[0][0] * matrix.m[1][1] * matrix.m[3][2] +
	                   matrix.m[0][1] * matrix.m[1][2] * matrix.m[3][0] +
	                   matrix.m[0][2] * matrix.m[1][0] * matrix.m[3][1] -
	                   matrix.m[0][2] * matrix.m[1][1] * matrix.m[3][0] -
	                   matrix.m[0][1] * matrix.m[1][0] * matrix.m[3][2] -
	                   matrix.m[0][0] * matrix.m[1][2] * matrix.m[3][1]) *
	                 determinantRecp;
	result.m[3][3] = (matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][2] +
	                  matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][0] +
	                  matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][1] -
	                  matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][0] -
	                  matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][2] -
	                  matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][1]) *
	                 determinantRecp;

	return result;
}

Matrix4x4 Lerp(const Matrix4x4& p1, const Matrix4x4& p2, float t) {
	Matrix4x4 result = {
	    (1.0f - t) * p1.m[0][0] + t * p2.m[0][0], (1.0f - t) * p1.m[0][1] + t * p2.m[0][1],
	     (1.0f - t) * p1.m[0][2] + t * p2.m[0][2], 0.0f,
	    (1.0f - t) * p1.m[1][0] + t * p2.m[1][0], (1.0f - t) * p1.m[1][1] + t * p2.m[1][1],
	     (1.0f - t) * p1.m[1][2] + t * p2.m[1][2], 0.0f,
	    (1.0f - t) * p1.m[2][0] + t * p2.m[2][0], (1.0f - t) * p1.m[2][1] + t * p2.m[2][1],
	     (1.0f - t) * p1.m[2][2] + t * p2.m[2][2], 0.0f,
	    (1.0f - t) * p1.m[3][0] + t * p2.m[3][0], (1.0f - t) * p1.m[3][1] + t * p2.m[3][1],
	     (1.0f - t) * p1.m[3][2] + t * p2.m[3][2], 1.0f,
	};

	return result;
}
