#pragma once
#include "Component.h"
#include "Objects/DynamicObjects/Player.h"

class PlayerAbility : public Component
{
public:
	PlayerAbility() : Component("PlayerAbility") {}
	
	void SetAbility(Player::Ability ability, float value);
	UINT GetAbility(Player::Ability ability);

private:
	UINT _hp = 100;
	UINT _mp = 100;
	UINT _moveSpeed = 150;
	UINT _str = 4;
	UINT _dex = 4;
	UINT _int = 4;
	UINT _luk = 4;
};