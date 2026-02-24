#include "stdafx.h"
#include "StaticObject.h"
#include "Components/RigidBody.h"

StaticObject::StaticObject(const std::string& name, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, float rotation)
	: Object(name, position, scale, rotation)
{
	// 물리 시뮬레이션에 의해 이동하지 않는 고정된 Static 타입의 RigidBody 추가
	auto rigidBody = std::make_shared<RigidBody>(BodyType::Static);
	AddComponent(rigidBody);

	// TODO : 충돌 처리를 위해 Collider가 필요하므로 Collider 컴포넌트 구현 후 여기서 함께 생성하도록 추가 예정
}
