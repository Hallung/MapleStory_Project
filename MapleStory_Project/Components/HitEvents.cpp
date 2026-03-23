#include "stdafx.h"
#include "HitEvents.h"
#include "Collider.h"
#include "Objects/Object.h"
#include "Transform.h"
#include "RigidBody.h"
#include "MonsterAbility.h"
#include "MonsterState.h"
#include "PlatformerController.h"


void HitEvents::Awake()
{
	// 현재 특별한 초기화 로직 없음
	// 필요 시 초기 상태 설정 가능
}

void HitEvents::Update()
{
	
}

void HitEvents::OnDestroy()
{
}

//=================================================
// 충돌 시작 이벤트
// 다른 Collider와 충돌이 시작될 때 호출된다.
// 1. 현재 충돌 중인 Collider 목록에 추가
// 2. Monster 레이어일 경우
//    - 가장 가까운 Monster를 추적 대상으로 갱신
// 가장 가까운 Monster 판단 기준:
// 플레이어 X 위치 기준 거리 비교
//=================================================
void HitEvents::OnCollisionEnter(Collider* self, Collider* other)
{
	if (!other || !self) return;
	
	// Ground Collider은 등록 X
	if (HasLayer(other->GetCollisionLayer(), CollisionLayer::Ground)) return;

	// 현재 충돌 중인 Collider 등록
	collidingMap[self].insert(other);

	// Monster와 충돌한경우만 처리 
	if (HasLayer(other->GetCollisionLayer(), CollisionLayer::Monster) &&
		HasLayer(self->GetCollisionLayer(), CollisionLayer::Player))
	{
		auto otherId = other->GetOwner()->GetComponent<RigidBody>("RigidBody")->GetBodyId();
		b2Vec2 otherPos = b2Body_GetPosition(otherId);

		// 아직 추적 대상이 없다면 바로 등록
		if (!b2Body_IsValid(nearestMonsterId))
		{
			nearestMonsterId = otherId;
			nearestMonsterPos = otherPos;
			return;
		}

		// 기존 대상과 거리 비교 후 더 가까운 Monster로 갱신
		auto ownerPos = b2Body_GetPosition(GetOwner()->GetComponent<RigidBody>("RigidBody")->GetBodyId());

		if (abs(ownerPos.x - otherPos.x) > abs(ownerPos.x - nearestMonsterPos.x))
		{
			nearestMonsterId = otherId;
			nearestMonsterPos = otherPos;
		}
	}
}

//======================================================================
// 충돌 종료 이벤트
// 다른 Collider와의 충돌이 끝날 때 호출된다.
// 1. 충돌 목록에서 Collider 제거
// 2. 종료된 Collider가 현재 추적 대상(closeId)이라면
//    → 추적 대상 초기화
//======================================================================
void HitEvents::OnCollisionExit(Collider* self, Collider* other)
{
	if (!other || !self) return;

	auto it = collidingMap.find(self);
	if (it != collidingMap.end())
	{
		it->second.erase(other);	// 충돌 목록에서 제거

		if (it->second.empty())
			collidingMap.erase(it);
	}

	// 현재 추적 중이던 Monster가 충돌 종료된 경우
	if (b2Body_IsValid(nearestMonsterId) &&
		B2_ID_EQUALS(
			other->GetOwner()->GetComponent<RigidBody>("RigidBody")->GetBodyId(), 
			nearestMonsterId))
	{
		// 추적 대상 초기화
		nearestMonsterId = b2_nullBodyId;
		nearestMonsterPos = kInvalidPosition;
	}
}

//======================================================================
// 특정 CollisionLayer와 충돌 중인지 확인하는 함수
// self 키값으로 currentColliders에 저장된 Collider들을 순회하며
// 지정한 Layer와 동일한 Layer가 존재하는지 검사한다.
//======================================================================
bool HitEvents::IsColliding(Collider* self, CollisionLayer otherLayer)
{
	auto it = collidingMap.find(self);
	if (it == collidingMap.end()) return false;

	for (auto col : it->second)
	{
		if (HasLayer(col->GetCollisionLayer(), otherLayer))
		{
			return true;
		}
	}
	return false;
}

//=======================================================================
// Layer 포함 여부 검사 함수
// CollisionLayer를 비트 플래그로 취급하여 특정 Layer가 포함되어 있는지 확인
//=======================================================================
bool HitEvents::HasLayer(CollisionLayer a, CollisionLayer b)
{
	return (a & b) != 0;
}

//=========================================================================
// self와 현재 충돌 중인 Collider들 중에서 특정 CollisionLayer에 속한 대상 중
// 가장 가까운 Coillider를 반환하는 함수
//=========================================================================
Collider* HitEvents::GetNearestTarget(Collider* self, CollisionLayer targetLayer)
{
	// self가 충돌 중인 목록 조회
	auto it = collidingMap.find(self);

	// 충돌 정보가 없으면 대상 없음
	if (it == collidingMap.end()) return nullptr;

	auto ownerRb = GetOwner()->GetComponent<RigidBody>("RigidBody");
	// 플레이어의 물리 월드 위치
	b2Vec2 ownerPos = b2Body_GetPosition(ownerRb->GetBodyId());

	Collider* nearest = nullptr;	// 가장 가까운 대상
	float minDistSq = FLT_MAX;		// 최소 거리 제곱값(초기값 매우 크게)

	// 현재 self와 충돌 중인 모든 Collider 검사
	for (auto col : it->second)
	{
		if (col)
		{
			// 대상의 원하는 충돌 레이어인지 확인
			if (HasLayer(col->GetCollisionLayer(), targetLayer))
			{
				auto rb = col->GetOwner()->GetComponent<RigidBody>("RigidBody");

				// 대상 위치
				b2Vec2 pos = b2Body_GetPosition(rb->GetBodyId());

				// 두 객체 간 거리 계산 (제곱 거리 사용 -> sqrt 연산 비용 절약)
				float dx = pos.x - ownerPos.x;
				float dy = pos.y - ownerPos.y;
				float distSq = dx * dx + dy * dy;

				// 현재까지 가장 가까운 대상인지 확인
				if (distSq < minDistSq)
				{
					minDistSq = distSq;
					nearest = col;
				}
			}
		}
	}
	return nearest;	// 가장 가까운 Collider 반환 (없으면 nullptr)
}

// collidingMap 내부에서 특정 Collider과 관련된 모든 충돌 정보를 제거하는 함수
void HitEvents::RemoveCollider(Collider* col)
{
	// unordered_map 전체 순회
	for (auto it = collidingMap.begin(); it != collidingMap.end(); )
	{
		// key 자체가 제거 대상이면 map에서 삭제
		if (it->first == col)
		{
			it = collidingMap.erase(it);
			continue;
		}

		// 내부에서도 해당 collider 제거
		it->second.erase(col);

		// 충돌 대상이 하나도 남지 않았다면 entery 제거
		if (it->second.empty())
			it = collidingMap.erase(it);
		else
			++it;
	}
}
