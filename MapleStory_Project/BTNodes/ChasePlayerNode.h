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

	enum class RecoveryType
	{
		None,
		Edge,
		Overlap
	};

	BTState Tick(float deltaTime) override
	{
		// 현재 어그로 대상 획득
		auto aggro = ai->GetOwner()->GetComponent<MonsterAggro>("MonsterAggro");
		auto collider = ai->GetOwner()->GetComponent<Collider>("Collider");

		// 타겟이 없으면 추격 불가 = 다른 행동으로 전환
		if (!aggro || !aggro->HasTarget()) return BTState::Failure;
		
		// 이동 및 상태 컴포넌트 획득
		auto movement = ai->GetOwner()->GetComponent<MonsterMovement>("MonsterMovement");
		auto state = ai->GetOwner()->GetComponent<MonsterState>("MonsterState");

		// 타겟과 자신의 Transform
		Transform* target = aggro->GetTarget();
		auto myTransform = ai->GetOwner()->GetTransform();

		float dist = target->GetPosition().x - myTransform->GetPosition().x;

		auto currentPos = myTransform->GetPosition();

		// 타겟 위치 기준 좌/우 방향 계산
		float playerDir = dist > 0 ? 1.0f : -1.0f;

		if (recovery != RecoveryType::None)
		{
			float moved = abs(currentPos.x - recoveryStartPos.x);

			if (moved >= recoveryDistance)
			{
				recovery = RecoveryType::None;
			}
			else
			{
				if (!collider->HasGroundAhead(recoveryDir))
				{
					recovery = RecoveryType::None;
					movement->Stop();
					return BTState::Running;
				}

				movement->Move(recoveryDir);
				state->SetState(Monster::State::CHASE);
				return BTState::Running;
			}
		}

		if (!collider->HasGroundAhead(playerDir))
		{
			recovery = RecoveryType::Edge;
			recoveryDir = -playerDir;
			recoveryStartPos = currentPos;

			return BTState::Running;
		}

		float absDist = abs(dist);

		const float overlapThreshold = 10.0f;

		if (absDist < overlapThreshold)
		{
			recovery = RecoveryType::Overlap;
			recoveryDir = playerDir;
			recoveryStartPos = currentPos;

			return BTState::Running;
		}

		// 해당 방향으로 이동
		movement->Move(playerDir);		

		// 몬스터 상태를 추격 상태로 변경
		state->SetState(Monster::State::CHASE);

		// 계속 추격 중
		return BTState::Running;
	}

private:
	MonsterAI* ai;

	float recoveryDir = 0.0f;
	float recoveryDistance = 50.0f;
	DirectX::SimpleMath::Vector2 recoveryStartPos;

	RecoveryType recovery = RecoveryType::None;
};