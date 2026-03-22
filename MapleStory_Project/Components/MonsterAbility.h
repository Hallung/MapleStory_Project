#pragma once
#include "Component.h"
#include "Collider.h"

class MonsterAbility : public Component
{
public:
	MonsterAbility() : Component("MonsterAbility") {}

	void SetHelthPoint(UINT hp);
	UINT GetHelthPoint() const { return _hp; }

	void SetMoveSpeed(float speed);
	void ReturnSpeed();
	UINT GetMoveSpeed() const { return _moveSpeed; }

	void SetAttackPower(UINT power);
	UINT GetAttackPower() const { return _attackPower; }

	void TakeDamage(Collider* other, UINT damage);

private:
	UINT _hp = 100;
	UINT _attackPower = 10;
	float _moveSpeed = 150.0f;

	float lastSpeed = 0.0f;
};