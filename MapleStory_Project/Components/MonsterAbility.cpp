#include "stdafx.h"
#include "MonsterAbility.h"
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
	UINT newhp = max(0, _hp - damage);

	SetHelthPoint(newhp);
}
