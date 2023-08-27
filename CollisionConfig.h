#pragma once

#include <cstdint>

//プレイヤー陣営
const uint32_t kCollisionAttributePlayer = 0b1;
//敵陣営
const uint32_t kCollisionAttributeEnemy = 0b1 << 1;
//ボス陣営
const uint32_t kCollisionAttributeBoss = 0b1 << 2;
//弾陣営
const uint32_t kCollisionAttributeBullet = 0b1 << 3;
