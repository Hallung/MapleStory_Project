#pragma once
#include "Monster.h"

class OrangeMushroom : public Monster
{
public:
	OrangeMushroom(DirectX::SimpleMath::Vector2 position,
		DirectX::SimpleMath::Vector2 scale,
		float rotation,
		const std::wstring& texturePath = L"_Textures/OrangeMushroom.png",
		BodyType bodyType = BodyType::Dynamic,
		const std::string& name = "OrangeMushroom");
	~OrangeMushroom() = default;
};