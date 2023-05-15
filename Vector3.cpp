#include "Vector3.h"


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
