#include "stdafx.h"
#include "PlayerAbility.h"

void PlayerAbility::SetAbility(Player::Ability ability, float value)
{
	switch (ability)
	{
	case Player::Ability::HP:
		_hp = value;
		break;
	case Player::Ability::MP:
		_mp = value;
		break;
	case Player::Ability::SPEED:
		_moveSpeed = value;
		break;
	case Player::Ability::STR:
		_str = 0;
		break;
	case Player::Ability::DEX:
		_dex = 0;
		break;
	case Player::Ability::INT:
		_int = 0;
		break;
	case Player::Ability::LUK:
		_luk = 0;
		break;
	}
}

UINT PlayerAbility::GetAbility(Player::Ability ability)
{
	UINT abilityValue = 0;

	switch (ability)
	{
	case Player::Ability::HP:
		abilityValue = _hp;
		break;
	case Player::Ability::MP:
		abilityValue = _mp;
		break;
	case Player::Ability::SPEED:
		abilityValue = _moveSpeed;
		break;
	case Player::Ability::STR:
		abilityValue = _str;
		break;
	case Player::Ability::DEX:
		abilityValue = _dex;
		break;
	case Player::Ability::INT:
		abilityValue = _int;
		break;
	case Player::Ability::LUK:
		abilityValue = _luk;
		break;
	default:
		break;
	}

	return abilityValue;
}
