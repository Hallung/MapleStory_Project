#pragma once
#include "stdafx.h"
#include "BTNode.h"
#include "Objects/Object.h"
#include "Components/Transform.h"
#include "Components/MonsterAI.h"
#include "Components/MonsterState.h"
#include "Components/MonsterMovement.h"
#include "Utilities/Random.h"

//==============================================
// 몬스터가 랜덤 방향으로 이동하는 배회 행동 노드
// Blackboard의 wanderState가 Move일 때만 실행
// 일정 시간 이동 후 Idle 상태로 전환
//==============================================
class WanderNode : public BTNode
{
public:
	WanderNode(MonsterAI* ai) : ai(ai) {}

	BTState Tick(float deltaTime) override
	{
		auto& blackboard = ai->GetBlackboard();

		// 지면 체크에 사용할 Collider
		auto collider = ai->GetOwner()->GetComponent<Collider>("Collider");

		// Move 상태가 아니면 실행 X
		if (blackboard.wanderState != MonsterBlackboard::WanderState::Move) 
			return BTState::Failure;

		// 이동 시간 감소
		blackboard.wanderTimer -= deltaTime;

		// 이동 애니메이션 상태 설정
		auto state = ai->GetOwner()->GetComponent<MonsterState>("MonsterState");
		state->SetState(Monster::State::MOVE);

		// 설정된 방향으로 이동
		auto movement = ai->GetOwner()->GetComponent<MonsterMovement>("MonsterMovement");
		movement->Move(blackboard.wanderDirection);

		// 이동 시간 종료 = Idle 전환
		if (blackboard.wanderTimer <= 0.0f)
		{
			blackboard.wanderTimer = 2.0f + Random::Range(0.0f, 300.0f) / 100.0f;

			blackboard.wanderState = MonsterBlackboard::WanderState::Idle;

			return BTState::Success;
		}

		// 앞쪽에 땅이 없을 경우 반대 방향으로 이동
		if (!collider->HasGroundAhead(blackboard.wanderDirection) &&
			!blackboard.recoveringFromEdge)
		{
			blackboard.wanderDirection *= -1.0f;
			blackboard.recoveringFromEdge = true;
		}
		else if (collider->HasGroundAhead(blackboard.wanderDirection) &&
			blackboard.recoveringFromEdge)
		{
			blackboard.recoveringFromEdge = false;
		}

		return BTState::Running;
	}

private:
	MonsterAI* ai;
};