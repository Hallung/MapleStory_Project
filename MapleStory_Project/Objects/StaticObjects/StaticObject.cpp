#include "stdafx.h"
#include "StaticObject.h"
#include "Components/RigidBody.h"

StaticObject::StaticObject(const std::string& name, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, float rotation, BodyType type)
	: Object(name, position, scale, rotation)
{
	// 지정된 BodyType으로 RigidBody 생성 (기본값: Static)
	rigidBody = std::make_shared<RigidBody>(type);
	AddComponent(rigidBody);

	// TODO : 충돌 처리를 위해 Collider가 필요하므로 Collider 컴포넌트 구현 후 여기서 함께 생성하도록 추가 예정
	// TODO : Collider 추가 후 충돌 이벤트 등 연동 예정
}
