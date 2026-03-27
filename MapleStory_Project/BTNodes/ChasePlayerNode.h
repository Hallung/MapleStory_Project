#pragma once
#include "stdafx.h"
#include "BTNode.h"
#include "Objects/Object.h"
#include "Components/MonsterAI.h"
#include "Components/HitEvents.h"
#include "Components/MonsterState.h"
#include "Components/MonsterAggro.h"
#include "Components/MonsterMovement.h"

// 몬스터가 Aggro를 가진 대상을 추격하는 노드
class ChasePlayerNode : public BTNode
{
public:
	ChasePlayerNode(MonsterAI* ai) : ai(ai) {}

	BTState Tick(float deltaTime) override
	{
		// 현재 어그로 대상 획득
		auto aggro = ai->GetOwner()->GetComponent<MonsterAggro>("MonsterAggro");

		// 타겟이 없으면 추격 불가 = 다른 행동으로 전환
		if (!aggro || !aggro->HasTarget()) return BTState::Failure;
		
		// 이동 및 상태 컴포넌트 획득
		auto movement = ai->GetOwner()->GetComponent<MonsterMovement>("MonsterMovement");
		auto state = ai->GetOwner()->GetComponent<MonsterState>("MonsterState");

		// 타겟과 자신의 Transform
		Transform* target = aggro->GetTarget();
		auto myTransform = ai->GetOwner()->GetTransform();

		// 타겟 위치 기준 좌/우 방향 계산
		float dir = (target->GetPosition().x - myTransform->GetPosition().x) > 0 ? 1.0f : -1.0f;

		// 해당 방향으로 이동
		movement->Move(dir);		

		// 몬스터 상태를 추격 상태로 변경
		state->SetState(Monster::State::CHASE);

		// 계속 추격 중
		return BTState::Running;
	}

private:
	MonsterAI* ai;
};