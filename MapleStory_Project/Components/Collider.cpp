#include "stdafx.h"
#include "Collider.h"
#include "Objects/Object.h"
#include "Components/Transform.h"
#include "Components/RigidBody.h"

namespace
{
// Collider 최소 크기 제한, 너무 작은 경우 Shape 생성 방지
constexpr float MIN_SIZE = 0.001f;
}

Collider::Collider()
	: Component("Collider")
{
	shapeId = b2_nullShapeId;
}

Collider::~Collider()
{
	if (b2Shape_IsValid(shapeId))
		b2DestroyShape(shapeId, true);
}

void Collider::Awake()
{
	RefreshShape(); // Rigidbody가 준비된 이후 Shape 생성
}

void Collider::Update()
{
	DirectX::SimpleMath::Vector2 currentScale = GetOwner()->GetTransform()->GetScale();

	// Scale이 변경된 경우 Refresh
	if (DirectX::SimpleMath::Vector2::DistanceSquared(currentScale, lastScale) > GetOwner()->GetTransform()->GetEpsilon())
		RefreshShape();
}

// Shape 생성 및 재생성 처리
void Collider::RefreshShape()
{
	auto rb = GetOwner()->GetComponent<RigidBody>("RigidBody");

	// Rigidbody가 없거나 Body가 유효하지 않으면 생성 불가
	if (rb == nullptr || b2Body_IsValid(rb->GetBodyId()) == false) return;

	DirectX::SimpleMath::Vector2 scale = GetOwner()->GetTransform()->GetScale();

	// 너무 작은 경우 Shape 생성하지 않음
	if (abs(scale.x) < MIN_SIZE || abs(scale.y) < MIN_SIZE) return;

	// 기존 Shape가 존재하는 경우 제거 처리
	if (b2Shape_IsValid(shapeId))
	{
		// 기존 Shape와 겹쳐있는 Collider들에게 Collision Exit 이벤트 전달
		b2AABB aabb = b2Shape_GetAABB(shapeId);
		b2World_OverlapAABB(PhysicsManager::GetInstance().GetWorldId(), aabb, b2DefaultQueryFilter(), NotifyExitCallback, this);

		// 기존 Shape 제거
		b2DestroyShape(shapeId, true);
		shapeId = b2_nullShapeId;
	}

	lastScale = scale;

	// Shape 기본 설정
	b2ShapeDef shapeDef = b2DefaultShapeDef();
	shapeDef.density = 1.0f;
	shapeDef.material.friction = 0.5f;
	shapeDef.material.restitution = 0.1f;

	// UserData에 Collider 저장, Collision 이벤트에서 사용
	shapeDef.userData = this;

	// Sensor 설정
	shapeDef.isSensor = isSensor;

	// Sensor 이벤트 활성화
	shapeDef.enableSensorEvents = true;

	// 일반 Collider는 Contact 이벤트 활성화
	if (!isSensor)
		shapeDef.enableContactEvents = true;

	// 실제 Shape 생성
	shapeId = CreateShapeInternal(rb->GetBodyId(), shapeDef, scale);

	// Body 깨우기
	b2Body_SetAwake(rb->GetBodyId(), true);
}

// Shape 제거 시 호출되는 Callback, 기존 충돌 상태를 종료시키기 위한 Exit 이벤트 전달
bool Collider::NotifyExitCallback(b2ShapeId otherShapeId, void* context)
{
	Collider* me = (Collider*)context;

	if (b2Shape_IsValid(otherShapeId) == false) return true;

	Collider* other = (Collider*)b2Shape_GetUserData(otherShapeId);

	// 상대 Collider가 존재하면 Exit 이벤트 전달
	if (other && other->GetOwner())
	{
		other->GetOwner()->OnCollisionExit(me); // 상대 Object 입장에서 Exit 호출
	}

	return false;
}
