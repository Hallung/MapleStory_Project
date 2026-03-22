#include "stdafx.h"
#include "MonsterAbility.h"
#include "MonsterState.h"
#include "HitEvents.h"
#include "Objects/DynamicObjects/Monster.h"

void MonsterAbility::SetHelthPoint(UINT hp)
{
	_hp = hp;
}

void MonsterAbility::SetMoveSpeed(float speed)
{
	lastSpeed = _moveSpeed;

	_moveSpeed = speed;
}

void MonsterAbility::ReturnSpeed()
{
	_moveSpeed = lastSpeed;
}

void MonsterAbility::SetAttackPower(UINT power)
{
	_attackPower = power;
}

void MonsterAbility::TakeDamage(Collider* other, UINT damage)
{
	std::cout << "Take Damage: " << damage << '\n';

	_hp = max(0, _hp - damage);

	auto state = GetOwner()->GetComponent<MonsterState>("MonsterState");

	auto hitEvent = other->GetOwner()->GetComponent<HitEvents>("HitEvents");

	auto monsterCol = GetOwner()->GetComponent<Collider>("SensorCollider");

	if (_hp == 0)
	{
		hitEvent->RemoveCollider(monsterCol.get());
		state->SetState(Monster::State::DIE);
	}
	else
	{
		state->SetState(Monster::State::HITTING);
	}
}
