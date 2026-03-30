#pragma once
#include "Components/RigidBody.h"

// 클리어 화면 이펙트 생성 클래스
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

	// 생성된 ClearEffect Object 반환
	std::shared_ptr<Object> GetClearEffect() const { return clearEffect; }

private:
	std::shared_ptr<Object> clearEffect;	// 실제 Sprite Object
	std::wstring texturePath;				// 텍스처 경로
	BodyType bodyType;						// 물리 Body 타입
	std::string name;						// Object 이름
};