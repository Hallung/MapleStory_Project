#include "stdafx.h"
#include "MonsterMovement.h"
#include "RigidBody.h"
#include "Transform.h"
#include "Objects/Object.h"

// 지정된 방향으로 몬스터 이동 처리
void MonsterMovement::Move(float dir)
{
	auto rigidBody = GetOwner()->GetComponent<RigidBody>("RigidBody");
	if (!rigidBody) return;

	//==============================================
	// 방향이 바뀌었을 때만 스프라이트 좌우 반전 수행
	// 매 프레임 반전되는 현상 방지
	//==============================================
	if (dir != facingDir)
	{
		facingDir = dir;

		auto transform = GetOwner()->GetTransform();
		auto scale = transform->GetScale();

		// x 스케일을 이용해 좌우 반전
		scale.x = abs(scale.x) * -dir;
		transform->SetScale(scale);
	}

	b2Vec2 velocity = b2Body_GetLinearVelocity(rigidBody->GetBodyId());
	// 현재 속도에 X축 이동 적용
	velocity.x = dir * moveSpeed * TimeManager::GetInstance().GetDeltaTime();

	b2Body_SetLinearVelocity(rigidBody->GetBodyId(), velocity);
}

// 몬스터의 수평 이동 정지
void MonsterMovement::Stop()
{
	auto rigidBody = GetOwner()->GetComponent<RigidBody>("RigidBody");
	if (!rigidBody) return;

	b2Vec2 velocity = b2Body_GetLinearVelocity(rigidBody->GetBodyId());
	velocity.x = 0.0f;

	b2Body_SetLinearVelocity(rigidBody->GetBodyId(), velocity);
}
