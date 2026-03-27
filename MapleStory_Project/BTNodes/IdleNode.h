#pragma once
#include "stdafx.h"
#include "BTNode.h"
#include "Objects/Object.h"
#include "Components/MonsterAI.h"
#include "Components/MonsterState.h"
#include "Components/MonsterMovement.h"
#include "Utilities/Random.h"

//=======================================================
// 몬스터가 일정 시간 동안 정지(대기) 상태를 유지하는 노드
// Blackboard의 wanderState가 Idle일 때만 실행
// 일정 시간이 지나면 Move 상태로 전환
//=======================================================
class IdleNode : public BTNode
{
public:
	IdleNode(MonsterAI* ai) : ai(ai) {}

	BTState Tick(float deltaTime) override
	{
		auto& blackboard = ai->GetBlackboard();

		// Idle 상태가 아니면 실행하지 않음
		if (blackboard.wanderState != MonsterBlackboard::WanderState::Idle)
			return BTState::Failure;

		// 대기 시간 감소
		blackboard.idleTimer -= TimeManager::GetInstance().GetDeltaTime();

		// 이동 정지
		auto movement = ai->GetOwner()->GetComponent<MonsterMovement>("MonsterMovement");
		movement->Stop();

		//애니메이션 상태 설정
		auto state = ai->GetOwner()->GetComponent<MonsterState>("MonsterState");
		state->SetState(Monster::State::STANDING);

		// 대기 시간 종료 = Wander 상태 전환
		if (blackboard.idleTimer <= 0.0f)
		{
			// 다음 Idle 시간 랜덤 설정
			blackboard.idleTimer = 2.0f + Random::Range(0.0f, 5.0f);

			// 이동 방향 랜덤 결정
			blackboard.wanderDirection = (Random::Range(0, 1) == 0) ? -1.0f : 1.0f;

			// 배회 상태로 변경
			blackboard.wanderState = MonsterBlackboard::WanderState::Move;

			return BTState::Success;
		}

		return BTState::Running;
	}

private:
	MonsterAI* ai;
};