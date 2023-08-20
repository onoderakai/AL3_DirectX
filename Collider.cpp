#include "Collider.h"

const float& Collider::GetRadius() { return radius_; }

void Collider::OnCollision() {}

Vector3 Collider::GetWorldPosition() { return Vector3(); }
