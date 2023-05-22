#include "Vector3.h"
#define _USE_MATH_DEFINES
#include <math.h>


Vector3 Normalize(const Vector3& v) {
	float length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	//Vector3 result = {(v.x * v.x) / length, (v.y * v.y) / length, (v.z * v.z) / length};
	Vector3 result = {v.x / length, v.y / length, v.z / length};
	/*if (v.x < 0.0f) {
		result.x *= -1;
	}
	if (v.y < 0.0f) {
		result.y *= -1;
	}
	if (v.z < 0.0f) {
		result.z *= -1;
	}*/
	return result;
}

Vector3& operator+=(Vector3& v1, const Vector3& v2) {
	v1.x += v2.x;
	v1.y += v2.y;
	v1.z += v2.z;

	return v1;
}

Vector3 operator+(const Vector3& v1, const Vector3& v2) {
	Vector3 result = v1;

	return result += v2;
}

Vector3& operator-=(Vector3& v1, const Vector3& v2) {
	v1.x -= v2.x;
	v1.y -= v2.y;
	v1.z -= v2.z;

	return v1;
}

Vector3 operator-(const Vector3& v1, const Vector3& v2) {
	Vector3 result = v1;

	return result -= v2;
}
