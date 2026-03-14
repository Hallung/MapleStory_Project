#pragma once
#include "Objects/DynamicObjects/Monster.h"

// 허수아비 몬스터 클래스
class Scarecrow : public Monster
{
public:
	Scarecrow(
		DirectX::SimpleMath::Vector2 position, 
		DirectX::SimpleMath::Vector2 scale, 
		float rotation, 
		const std::wstring& texturePath = L"_Textures/Scarecrow.png", 
		BodyType bodyType = BodyType::Dynamic, 
		const std::string& name = "Scarecrow");
};