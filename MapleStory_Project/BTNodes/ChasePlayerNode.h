#pragma once
#include "stdafx.h"
#include "BTNode.h"
#include "Objects/Object.h"
#include "Components/MonsterAI.h"
#include "Components/HitEvents.h"
#include "Components/MonsterState.h"
#include "Components/MonsterAggro.h"
#include "Components/MonsterMovement.h"

//==========================================
// 몬스터가 Aggro를 가진 대상을 추격하는 노드
// - 기본적으로 플레이어 방향으로 이동
// - 낭떠러지 감지 시 후퇴
// - 플레이어와 겹쳤을 때 위치 보정
//==========================================
class ChasePlayerNode : public BTNode
{
public:
	ChasePlayerNode(MonsterAI* ai) : ai(ai) {}

	// 추격 중 발생할 수 있는 보정 상태
	enum class RecoveryType
	{
		None,	// 정상 추격 상태
		Edge,	// 앞에 지면이 없으면 뒤로 이동
		Overlap	// 플레이어와 너무 까우면 위치 분리
	};

	BTState Tick(float deltaTime) override
	{
		// 현재 어그로 대상 획득
		auto aggro = ai->GetOwner()->GetComponent<MonsterAggro>("MonsterAggro");

		// 지면 체크 및 전방 검사에 사용할 Collider
		auto collider = ai->GetOwner()->GetComponent<Collider>("Collider");

		// 타겟이 없으면 추격 불가 = 다른 행동으로 전환
		if (!aggro || !aggro->HasTarget()) return BTState::Failure;
		
		// 이동 및 상태 컴포넌트 획득
		auto movement = ai->GetOwner()->GetComponent<MonsterMovement>("MonsterMovement");
		auto state = ai->GetOwner()->GetComponent<MonsterState>("MonsterState");

		// 타겟과 자신의 Transform
		Transform* target = aggro->GetTarget();
		auto myTransform = ai->GetOwner()->GetTransform();

		// 플레이어와의 X축 거리
		float dist = target->GetPosition().x - myTransform->GetPosition().x;

		auto currentPos = myTransform->GetPosition();

		// 타겟 위치 기준 좌/우 방향 계산
		float playerDir = dist > 0 ? 1.0f : -1.0f;

		// Recovery 상태 처리 (Edge / Overlap 보정 이동)
		if (recovery != RecoveryType::None)
		{
			// Recovery 시작 지점으로부터 이동한 거리
			float moved = abs(currentPos.x - recoveryStartPos.x);

			// 일정 거리 이상 이동했다면 보정 종료
			if (moved >= recoveryDistance)
			{
				recovery = RecoveryType::None;
			}
			else
			{
				// 진행 방향 앞에 지면이 없으면 즉시 중단
				if (!collider->HasGroundAhead(recoveryDir))
				{
					recovery = RecoveryType::None;
					movement->Stop();
					return BTState::Running;
				}

				// 보정 방향으로 이동 지속
				movement->Move(recoveryDir);
				// 여전히 추격 상태 유지
				state->SetState(Monster::State::CHASE);
				return BTState::Running;
			}
		}

		//=======================================================================
		// 낭떠러지 감지
		// 플레이어 방향으로 이동 시 떨어질 위험이 있으면 반대 방향으로 Recovery 시작
		//=======================================================================
		if (!collider->HasGroundAhead(playerDir))
		{
			recovery = RecoveryType::Edge;	// Edge Recovery 진입
			recoveryDir = -playerDir;		// 반대 방향 이동
			recoveryStartPos = currentPos;	// 시작 위치 저장

			return BTState::Running;
		}

		float absDist = abs(dist);

		// 플레이어와 너무 가까운 거리 기준값
		const float overlapThreshold = 10.0f;

		//=========================================
		// 플레이어와 겹침 방지
		// 너무 가까우면 살짝 밀어내는 Recovery 실행
		//=========================================
		if (absDist < overlapThreshold)
		{
			recovery = RecoveryType::Overlap;
			recoveryDir = playerDir;			// 플레이어 방향으로 밀어내기
			recoveryStartPos = currentPos;

			return BTState::Running;
		}

		// 정상 추격 로직

		// 해당 방향으로 이동
		movement->Move(playerDir);		

		// 몬스터 상태를 추격 상태로 변경
		state->SetState(Monster::State::CHASE);

		// 계속 추격 중
		return BTState::Running;
	}

private:
	MonsterAI* ai;	// 이 노드를 소유한 AI

	float recoveryDir = 0.0f;	// Recovery 이동 방향
	float recoveryDistance = 50.0f;	// Recovery 상태에서 이동할 최대 거리
	DirectX::SimpleMath::Vector2 recoveryStartPos;	// Recovery 시작 위치

	RecoveryType recovery = RecoveryType::None;	// 현재 Recovery 상태
};