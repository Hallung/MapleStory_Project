#pragma once
#include "Components/RigidBody.h"

// 포탈 오브젝트 생성 클래스
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

	// 생성된 Portal Object 반환
	std::shared_ptr<Object> GetPortal() const { return portal; }

private:
	std::shared_ptr<Object> portal;		// Sprite Object
	std::wstring texturePath;			// 텍스처 경로
	BodyType bodyType;					// 물리 Body 타입
	std::string name;					// Object 이름
};