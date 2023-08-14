#include "Vector2.h"

Vector2& operator+=(Vector2& v1, const Vector2& v2) {
	v1.x += v2.x;
	v1.y += v2.y;

	return v1;
}

Vector2 operator+(const Vector2& v1, const Vector2& v2) {
	Vector2 result = v1;

	return result += v2;
}

Vector2& operator-=(Vector2& v1, const Vector2& v2) {
	v1.x -= v2.x;
	v1.y -= v2.y;

	return v1;
}

Vector2 operator-(const Vector2& v1, const Vector2& v2) {
	Vector2 result = v1;

	return result -= v2;
}

Vector2 operator*(const Vector2& v, const float& value) {
	Vector2 vector = v;
	vector.x *= value;
	vector.y *= value;
	return vector;
}
