#include "stdafx.h"
#include "RigidBody.h"
#include "Objects/Object.h"
#include "Transform.h"
#include "Utilities/PhysicsUtils.h"

// Box2D 기본 바디 정의를 생성하고 타입을 설정
RigidBody::RigidBody(BodyType type, const std::string& name)
	:Component(name)
{
	// Box2D 기본 바디 정의 초기화
	bodyDef = b2DefaultBodyDef();
	// 바디 타입 설정
	bodyDef.type = (b2BodyType)type;
}

// 소멸 시 Box2D 월드에서 바디 제거
RigidBody::~RigidBody()
{
	if (b2Body_IsValid(bodyId))
	{
		b2DestroyBody(bodyId);
		bodyId = b2_nullBodyId;
	}
}

// Transform 정보를 기반으로 Box2D 바디를 생성하고 월드에 등록
void RigidBody::Awake()
{
	auto transform = GetOwner()->GetTransform();

	// 화면 좌표 -> 물리 월드 좌표 변환
	bodyDef.position = PhysicsUtils::ScreenToWorld(transform->GetPosition());
	// 엔진 회전(시계) -> Box2D 회전(반시계) 좌표계 보정
	bodyDef.rotation = b2MakeRot(-transform->GetRotationRadian());
	// 충돌 콜백 등에서 사용할 사용자 데이터
	bodyDef.userData = GetOwner();

	// 물리 월드에 바디 생성
	b2WorldId worldId = PhysicsManager::GetInstance().GetWorldId();
	if (b2World_IsValid(worldId))
		bodyId = b2CreateBody(worldId, &bodyDef);
}

//========================================
// 매 프레임 Transform 동기화
// Box2D 시뮬레이션 결과를 Transform에 반영
//========================================
void RigidBody::Update()
{
	// Static 바디는 움직이지 않으며, 유효하지 않은 바디는 return
	if (bodyDef.type == b2_staticBody || !b2Body_IsValid(bodyId)) return;

	// Box2D 위치/회전 가져오기
	b2Vec2 position = b2Body_GetPosition(bodyId);
	b2Rot rotation = b2Body_GetRotation(bodyId);
	float angle = b2Rot_GetAngle(rotation);

	auto transform = GetOwner()->GetTransform();
	// 물리 월드 -> 화면 좌표 변환 후 Transform 반영
	transform->SetPosition(PhysicsUtils::WorldToScreen(position));
	// Box2D(반시계) -> 엔진(시계) 회전 보정
	transform->SetRotationRadian(-angle);
}

// 화면 좌표 속도를 물리 월드 좌표로 변환하여 적용
void RigidBody::SetVelocity(DirectX::SimpleMath::Vector2 velocity)
{
	if (b2Body_IsValid(bodyId))
	{
		b2Body_SetLinearVelocity(bodyId, PhysicsUtils::ScreenToWorld(velocity));
	}
}

//===============================================================
// 감쇠 계수 설정
// 바디 생성 전/후 모두 반영되도록 bodyDef와 실제 바디에 동시에 적용
//===============================================================
void RigidBody::SetDamping(float linear, float angular)
{
	bodyDef.linearDamping = linear;
	bodyDef.angularDamping = angular;

	if (b2Body_IsValid(bodyId))
	{
		b2Body_SetLinearDamping(bodyId, linear);
		b2Body_SetAngularDamping(bodyId, angular);
	}
}

//========================================
// 회전 고정 여부 설정
// 물리 시뮬레이션 중 회전 가능 여부를 제어
//========================================
void RigidBody::SetFixedRotation(bool fixed)
{
	bodyDef.fixedRotation = fixed;
	if (b2Body_IsValid(bodyId))
		b2Body_SetFixedRotation(bodyId, fixed);
}
