#include "stdafx.h"
#include "Collider.h"
#include "Objects/Object.h"
#include "Transform.h"
#include "RigidBody.h"
#include "HitEvents.h"
#include "PlatformerController.h"
#include "Utilities/PhysicsUtils.h"

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
	if (DirectX::SimpleMath::Vector2::DistanceSquared(DirectX::XMVectorAbs(currentScale), DirectX::XMVectorAbs(lastScale)) > GetOwner()->GetTransform()->GetEpsilon())
		RefreshShape();
}

void Collider::OnDestroy()
{
	for (auto& shapeId : shapeIds)
	{
		if (b2Shape_IsValid(shapeId))
			b2DestroyShape(shapeId, true);
	}
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

	// Shape의 실제 크기와 별개로 Object 크기 변화 시
	// 사용하기 위한 ownerScale
	DirectX::SimpleMath::Vector2 ownerScale 
		= GetOwner()->GetTransform()->GetScale();

	// 너무 작은 경우 Shape 생성하지 않음
	if (abs(ownerScale.x) < MIN_SIZE || abs(ownerScale.y) < MIN_SIZE) return;

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

	lastScale = ownerScale;

	// Shape 기본 설정
	b2ShapeDef shapeDef = b2DefaultShapeDef();

	// isSensor 상태에 따라 density, friction, restitution 설정
	if (isSensor)
	{
		shapeDef.density = 0.0f;
		shapeDef.material.friction = 0.0f;
		shapeDef.material.restitution = 0.0f;
	}
	else
	{
		shapeDef.density = 1.0f;
		shapeDef.material.friction = 1.0f;
		shapeDef.material.restitution = 0.0f;
	}

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
	if (scale.x <= 0.0f || scale.y <= 0.0f)
		CreateShapes(rb->GetBodyId(), shapeDef, ownerScale);
	else
		CreateShapes(rb->GetBodyId(), shapeDef, Collider::scale);

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
	Collider* me = static_cast<Collider*>(context);

	if (!b2Shape_IsValid(otherShapeId)) return true;

	Collider* other = static_cast<Collider*>(b2Shape_GetUserData(otherShapeId));

	// 상대 Collider가 존재하면 Exit 이벤트 전달
	if (other && other->GetOwner())
	{
		other->GetOwner()->OnCollisionExit(other, me); // 상대 Object 입장에서 Exit 호출
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

// 플레이어 (또는 이 Collider를 가진 객체)가 지면에 닿아 있는지 확인하는 함수
bool Collider::CheckGrounded()
{
	auto ownerRb = GetOwner()->GetComponent<RigidBody>("RigidBody");

	auto ownerPosition = b2Body_GetPosition(ownerRb->GetBodyId());

	// 객체 높이의 절반 (캐릭터 중심 기준으로 발 위치를 계산할 때 사용)
	b2AABB aabb = b2Shape_GetAABB(shapeIds.front());
	float halfHeight = (aabb.upperBound.y - aabb.lowerBound.y) * 0.5f;
	float halfWidth = (aabb.upperBound.x - aabb.lowerBound.x) * 0.5f;

	// Ray가 검사할 최대 거리
	float totalDistance = 0.3f;

	float skin = 0.03f;

	// RayCast 시작 위치
	b2Vec2 origin = { ownerPosition.x , ownerPosition.y - halfHeight + skin};

	//=============================================================
	// PhysicsManager의 Raycast를 호출하여 아래 방향으로 Ray를 발사
	// origin : Ray 시작 위치
	// {0, -1} : 아래 방향 (Down)
	// totalDistance : Ray 길이
	// CollisionLayer::Ground : Ground 레이어만 충돌 검사
	//=============================================================
	RaycastHit centerHit = PhysicsManager::GetInstance().Raycast(origin, { 0, -1 }, totalDistance, (uint32_t)CollisionLayer::Ground);

	b2Vec2 rightOrigin = { origin.x + halfWidth , origin.y };
	RaycastHit rightHit = PhysicsManager::GetInstance().Raycast(rightOrigin, { 0, -1 }, totalDistance, (uint32_t)CollisionLayer::Ground);

	b2Vec2 leftOrigin = { origin.x - halfWidth , origin.y };
	RaycastHit leftHit = PhysicsManager::GetInstance().Raycast(leftOrigin, { 0, -1 }, totalDistance, (uint32_t)CollisionLayer::Ground);

	bool hit = false;

	if (centerHit.hit || rightHit.hit || leftHit.hit) hit = true;
	else hit = false;

	float velocityY = b2Body_GetLinearVelocity(ownerRb->GetBodyId()).y;

	const float landingVelocityThreshold = 1.4f;

	bool validLanding = velocityY <= landingVelocityThreshold;

	float groundNormalY = 0.0f;
	if (centerHit.hit) groundNormalY = centerHit.normal.y;
	else if (rightHit.hit) groundNormalY = rightHit.normal.y;
	else if (leftHit.hit) groundNormalY = leftHit.normal.y;

	// Ray가 Ground Collider와 충돌했다면 true (지면에 닿아 있음)
	// 충돌이 없으면 false (공중 상태)
	return hit && validLanding && groundNormalY > 0.6f;
}

bool Collider::HasGroundAhead(float dir)
{
	auto ownerRb = GetOwner()->GetComponent<RigidBody>("RigidBody");
	b2Vec2 ownerPos = b2Body_GetPosition(ownerRb->GetBodyId());

	float halfHeight = scale.y * 0.5f;
	float halfWidth = scale.x * 0.5f;

	float forwardOffset = halfWidth + 5.0f;

	b2Vec2 origin =
	{
		ownerPos.x + dir * forwardOffset,
		ownerPos.y
	};

	float checkDistance = halfHeight + 0.2f;

	RaycastHit hit = PhysicsManager::GetInstance().Raycast(
		origin, { 0, -1 }, checkDistance, (uint32_t)CollisionLayer::Ground
	);

	return hit.hit;
}
