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
// 해당 Collider를 currentColliders 목록에 추가하여
// 현재 충돌 상태를 기록한다.
//=================================================
void HitEvents::OnCollisionEnter(Collider* other)
{
	if (!other) return;

	currentColliders.insert(other);	// 충돌 중인 Collider 목록에 추가
}

//======================================================================
// 충돌 종료 이벤트
// 다른 Collider와의 충돌이 끝날 때 호출된다.
// 해당 Collider를 currentColliders 목록에서 제거한다.
//======================================================================
void HitEvents::OnCollisionExit(Collider* other)
{
	if (!other) return;
	currentColliders.erase(other);	// 충돌 목록에서 제거
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
