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

Collider::Collider(std::string name)
	: Component(name)
{
}

Collider::~Collider()
{
	// Collider 소멸시 Shape 해제
	DestroyShapes();
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

void Collider::SetCollisionLayer(CollisionLayer layer)
{
	// 동일한 값이면 불필요한 업데이트 방지
	if (this->layer == layer) return;

	// Collision Layer 변경
	this->layer = layer;

	// Box2D Filter에 즉시 반영
	ApplyFilter();
}

void Collider::SetCollisionMask(uint32_t mask)
{
	// 동일한 값이면 불필요한 업데이트 방지
	if (this->mask == mask) return;

	// Collision Mask 변경
	this->mask = mask;

	// Box2D Filter에 즉시 반영
	ApplyFilter();
}

// Shape 생성 및 재생성 처리
void Collider::RefreshShape()
{
	auto rb = GetOwner()->GetComponent<RigidBody>("RigidBody");

	// Rigidbody가 없거나 Body가 유효하지 않으면 생성 불가
	if (!rb || !b2Body_IsValid(rb->GetBodyId())) return;

	//===========================================
	// Collider Scale 결정 로직
	// - ColliderScale이 설정되지 않았을 경우
	//   Owner의 Transform Scale을 사용
	// - ColliderScale이 설정된 경우
	//   해당 값을 충돌 크기로 사용
	//===========================================
	if (scale.x < MIN_SIZE && scale.y < MIN_SIZE)
		scale = GetOwner()->GetTransform()->GetScale();

	// 너무 작은 경우 Shape 생성하지 않음
	if (abs(scale.x) < MIN_SIZE || abs(scale.y) < MIN_SIZE) return;

	// 기존 Shape가 존재하는 경우 제거 처리
	for (auto id : shapeIds)
	{
		if (b2Shape_IsValid(id))
		{
			// 기존 Shape와 겹쳐있는 Collider들에게 Collision Exit 이벤트 전달
			b2AABB aabb = b2Shape_GetAABB(id);
			b2World_OverlapAABB(
				PhysicsManager::GetInstance().GetWorldId(), 
				aabb, 
				b2DefaultQueryFilter(), 
				NotifyExitCallback, 
				this);

			// 기존 Shape 제거
			b2DestroyShape(id, true);
		}
	}
	shapeIds.clear();

	lastScale = scale;

	// Shape 기본 설정
	b2ShapeDef shapeDef = b2DefaultShapeDef();
	shapeDef.density = 1.0f;
	shapeDef.material.friction = 0.5f;
	shapeDef.material.restitution = 0.0f;

	// UserData에 Collider 저장, Collision 이벤트에서 사용
	shapeDef.userData = this;

	// Sensor 설정
	shapeDef.isSensor = isSensor;

	// Sensor 이벤트 활성화
	shapeDef.enableSensorEvents = true;

	// 일반 Collider는 Contact 이벤트 활성화
	if (!isSensor)
		shapeDef.enableContactEvents = true;

	// Collision Filter 설정, 이를 통해 Layer 기반 Collision Filtering 수행
	shapeDef.filter.categoryBits = static_cast<uint32_t>(layer);
	shapeDef.filter.maskBits = mask;

	// 실제 Shape 생성
	CreateShapes(rb->GetBodyId(), shapeDef, scale);

	// Body 깨우기
	b2Body_SetAwake(rb->GetBodyId(), true);
}

// Shape가 존재하면 shapeIds를 순회하며 모두 해제 후 vector 비우기
void Collider::DestroyShapes()
{
	for (auto id : shapeIds)
	{
		if (b2Shape_IsValid(id))
			b2DestroyShape(id, true);
	}
	shapeIds.clear();
}

// Shape 제거 시 호출되는 Callback, 기존 충돌 상태를 종료시키기 위한 Exit 이벤트 전달
bool Collider::NotifyExitCallback(b2ShapeId otherShapeId, void* context)
{
	Collider* me = (Collider*)context;

	if (!b2Shape_IsValid(otherShapeId)) return true;

	Collider* other = (Collider*)b2Shape_GetUserData(otherShapeId);

	// 상대 Collider가 존재하면 Exit 이벤트 전달
	if (other && other->GetOwner())
	{
		other->GetOwner()->OnCollisionExit(me); // 상대 Object 입장에서 Exit 호출
	}

	return false;
}

void Collider::ApplyFilter() const
{
	b2Filter filter = b2DefaultFilter(); // 기본 Filter 생성

	filter.categoryBits = static_cast<uint32_t>(layer); // 이 Collider가 속한 Layer
	filter.maskBits = mask; // 이 Collider가 충돌을 허용할 Layer
	filter.groupIndex = 0; // 같은 그룹 간 충돌 제어용 (현재 사용 안함)

	// 전체 shapeId를 돌면서 필터 적용
	for (auto id : shapeIds)
	{
		if (b2Shape_IsValid(id))
			b2Shape_SetFilter(id, filter); // Box2D Shape에 Filter 적용
	}
}
