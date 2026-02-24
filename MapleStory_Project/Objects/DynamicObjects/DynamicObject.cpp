#include "stdafx.h"
#include "DynamicObject.h"
#include "Components/RigidBody.h"

DynamicObject::DynamicObject(const std::string& name, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, float rotation)
	: Object(name, position, scale, rotation)
{
	// 물리 시뮬레이션에 의해 움직이는 Dynamic 타입의 RigidBody 추가
	rigidBody = std::make_shared<RigidBody>(BodyType::Dynamic);
	AddComponent(rigidBody);

	// TODO : 충돌 처리를 위해 Collider가 필요하므로 Collider 컴포넌트 구현 후 여기서 함께 생성하도록 추가 예정
	// TODO : Collider 추가 후 충돌 이벤트 등 연동 예정
}
