#pragma once
#include "Components/RigidBody.h"

class ClearEffect
{
public:
	ClearEffect(
		DirectX::SimpleMath::Vector2 position,
		DirectX::SimpleMath::Vector2 scale,
		float rotation,
		const std::wstring& texturePath = L"_Textures/Clear.png",
		BodyType bodyType = BodyType::Static,
		const std::string& name = "ClearEffect"
	);
	~ClearEffect() = default;

	std::shared_ptr<Object> GetClearEffect() const { return clearEffect; }

private:
	std::shared_ptr<Object> clearEffect;
	std::wstring texturePath;
	BodyType bodyType;
	std::string name;
};