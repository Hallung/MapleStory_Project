#include "stdafx.h"
#include "HitEvents.h"
#include "Collider.h"
#include "Objects/Object.h"
#include "Transform.h"
#include "RigidBody.h"


void HitEvents::Awake()
{
	// 현재 특별한 초기화 로직 없음
	// 필요 시 초기 상태 설정 가능
}

void HitEvents::Update()
{
	// 현재 프레임에서 특별한 업데이트 로직 없음
	// 향후 충돌 지속 시간 관리, 디버깅, 상태 체크 등을
	// 추가할 수 있는 확장 포인트
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
void HitEvents::OnCollisionEnter(Collider* other)
{
	if (!other) return;

	// TODO: enum 연산자 오버로드(&) 추가하기
	// 추가 후 CollisionLayer() == 를 변경
	
	// Ground Collider은 등록 X
	if (other->GetCollisionLayer() == CollisionLayer::Ground) return;

	// 현재 충돌 중인 Collider 등록
	currentColliders.insert(other);	// 충돌 중인 Collider 목록에 추가

	// Monster와 충돌한경우만 처리 
	if (other->GetCollisionLayer() == CollisionLayer::Monster)
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
void HitEvents::OnCollisionExit(Collider* other)
{
	if (!other) return;

	currentColliders.erase(other);	// 충돌 목록에서 제거

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
// currentColliders에 저장된 Collider들을 순회하며
// 지정한 Layer와 동일한 Layer가 존재하는지 검사한다.
//======================================================================
bool HitEvents::IsCollidingWith(CollisionLayer layer) const
{
	for (auto col : currentColliders)
	{
		// Collider 존재 확인 후 Layer 비교
		if (col && col->GetCollisionLayer() == layer)
		{
			return true;
		}
	}
	return false;	// 해당 Layer와 충돌 중인 Collider가 없음
}
