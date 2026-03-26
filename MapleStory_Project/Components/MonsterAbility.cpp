#include "stdafx.h"
#include "MonsterAbility.h"
#include "MonsterState.h"
#include "HitEvents.h"
#include "MonsterAggro.h"
#include "Objects/DynamicObjects/Monster.h"

void MonsterAbility::SetHelthPoint(UINT hp)
{
	_hp = hp;
}

// 이동 속도 변경(기존 속도를 저장하여 이후 복귀에 활용)
void MonsterAbility::SetMoveSpeed(float speed)
{
	lastSpeed = _moveSpeed;

	_moveSpeed = speed;
}

// 이전 이동 속도로 복구
void MonsterAbility::ReturnSpeed()
{
	_moveSpeed = lastSpeed;
}

void MonsterAbility::SetAttackPower(UINT power)
{
	_attackPower = power;
}

// 몬스터에게 데미지를 적용하는 함수
void MonsterAbility::TakeDamage(Collider* other, UINT damage)
{
	// 체력 감소 (0 이하 방지)
	_hp = max(0, _hp - damage);

	auto state = GetOwner()->GetComponent<MonsterState>("MonsterState");
	auto hitEvent = other->GetOwner()->GetComponent<HitEvents>("HitEvents");
	auto monsterCol = GetOwner()->GetComponent<Collider>("SensorCollider");
	auto aggro = GetOwner()->GetComponent<MonsterAggro>("MonsterAggro");

	// Damage가 들어오면 공격을 준 Collider 저장
	if (aggro) 
	{
		aggro->SetAggro(other->GetOwner());
	}

	// 사망 처리
	if (_hp == 0)
	{
		// 충돌 목록에서 제거 (추가 피격 방지)
		hitEvent->RemoveCollider(monsterCol.get());
		// 사망 상태 진입
		state->SetState(Monster::State::DIE);
	}
	else
	{
		// 피격 상태 진입
		state->SetState(Monster::State::HITTING);
	}
}
