#pragma once
#include "Components/RigidBody.h"

class Portal
{
public:
	Portal(
		DirectX::SimpleMath::Vector2 position, 
		DirectX::SimpleMath::Vector2 scale, 
		float rotation, 
		const std::wstring& texturePath = L"_Textures/Portal.png", 
		BodyType bodyType = BodyType::Static, 
		const std::string& name = "Portal"
	);
	~Portal() = default;

	std::shared_ptr<Object> GetPortal() const { return portal; }

private:
	std::shared_ptr<Object> portal;
	std::wstring texturePath;
	BodyType bodyType;
	std::string name;
};