#include "stdafx.h"
#include "MonsterAbility.h"
#include "MonsterState.h"
#include "HitEvents.h"
#include "MonsterAggro.h"
#include "Objects/DynamicObjects/Monster.h"

void MonsterAbility::Awake()
{
	monsterCollider = GetOwner()->GetComponent<Collider>("SensorCollider");
	monsterState = GetOwner()->GetComponent<MonsterState>("MonsterState");
	monsterAggro = GetOwner()->GetComponent<MonsterAggro>("MonsterAggro");
}

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
	if (_hp <= damage)
		_hp = 0;
	else
		_hp = _hp - damage;

	if (soundPath != "")
		SoundManager::GetInstance().PlaySFX(soundPath);

	auto hitEvent = other->GetOwner()->GetComponent<HitEvents>("HitEvents");

	// Damage가 들어오면 공격을 준 Collider 저장
	if (monsterAggro)
	{
		monsterAggro->SetAggro(other->GetOwner());
	}

	// 사망 처리
	if (_hp == 0)
	{
		// 충돌 목록에서 제거 (추가 피격 방지)
		hitEvent->RemoveCollider(monsterCollider.get());
		// 사망 상태 진입
		monsterState->SetState(Monster::State::DIE);
	}
	else
	{
		// 피격 상태 진입
		monsterState->SetState(Monster::State::HITTING);
	}
}
