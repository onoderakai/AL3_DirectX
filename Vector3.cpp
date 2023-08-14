#include "Vector3.h"
#define _USE_MATH_DEFINES
#include <math.h>


Vector3 Normalize(const Vector3& v) {
	float length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	if (length == 0.0f) {
		return v;
	}
	Vector3 result = {v.x / length, v.y / length, v.z / length};
	return result;
}

Vector3 Lerp(const Vector3& p1, const Vector3& p2, float t) {
	return {
	    (1.0f - t) * p1.x + t * p2.x, (1.0f - t) * p1.y + t * p2.y, (1.0f - t) * p1.z + t * p2.z};
}

Vector3 Slerp(const Vector3& p1, const Vector3& p2, float t) {
	return {
	    (1.0f - t) * p1.x + t * p2.x, (1.0f - t) * p1.y + t * p2.y, (1.0f - t) * p1.z + t * p2.z};
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

Vector3 operator*(const Vector3& v, const float& value) {
	Vector3 vector = v;
	vector.x *= value;
	vector.y *= value;
	vector.z *= value;
	return vector;
}
