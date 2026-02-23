#pragma once
#include "stdafx.h"

namespace PhysicsUtils
{
constexpr float PTM_RATIO = 50.0f;

inline b2Vec2 ScreenToWorld(DirectX::SimpleMath::Vector2 screenPos)
{
	return b2Vec2{ screenPos.x / PTM_RATIO, screenPos.y / PTM_RATIO };
}

inline DirectX::SimpleMath::Vector2 WorldToScreen(b2Vec2 worldPos)
{
	return DirectX::SimpleMath::Vector2{ worldPos.x * PTM_RATIO, worldPos.y * PTM_RATIO };
}
}