#include "stdafx.h"
#include "MonsterAbility.h"
#include "MonsterState.h"
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

void MonsterAbility::TakeDamage(UINT damage)
{
	std::cout << "Take Damage: " << damage << '\n';

	_hp = max(0, _hp - damage);

	auto state = GetOwner()->GetComponent<MonsterState>("MonsterState");

	if (_hp == 0)
		state->SetState(Monster::State::DIE);
	else
		state->SetState(Monster::State::HITTING);
}
