#pragma once
#include "stdafx.h"

// 각 Collider가 속하는 "레이어"를 나타내는 비트 플래그 enum
// uint32_t를 사용하여 최대 32개의 서로 다른 레이어 정의 가능
// 각 레이어는 하나의 비트를 사용 (1 << n 형태)
enum class CollisionLayer : uint32_t
{
	Default = 0x0001, // 기본 레이어 (0000 0001)
	Ground = 0x0002,  // 지형 (0000 0010)
	Player = 0x0004,  // 플레이어 (0000 0100)
	Monster = 0x0008, // 몬스터 (0000 1000)
	Bullet = 0x0010,  // 총알 (0001 0000)
	UI = 0x0020,      // UI (0010 0000)
};

// 두 개의 CollisionLayer를 OR 연산하여 여러 레이어를 동시에 표현할 수 있도록 지원
inline uint32_t operator|(CollisionLayer a, CollisionLayer b)
{
	return static_cast<uint32_t>(a) | static_cast<uint32_t>(b);
}

// 기존 mask 값에 CollisionLayer를 추가할 수 있도록 지원
inline uint32_t operator|(uint32_t a, CollisionLayer b)
{
	return a | static_cast<uint32_t>(b);
}