#include "stdafx.h"
#include "Player.h"
#include "Components/PlatformerController.h"
#include "Components/Transform.h"
#include "Utilities/ObjectFactory.h"

Player::Player(DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, float rotation, const std::wstring& texturePath, BodyType bodyType, const std::string& name)
	: bodyType(bodyType), texturePath(texturePath)
{
	auto player = ObjectFactory::CreateSprite(
			DirectX::SimpleMath::Vector2(gWinWidth * 0.5f, gWinHeight * 0.5f),
			DirectX::SimpleMath::Vector2(scale),
			rotation,
			texturePath
		);
	player->AddComponent(std::make_shared<RigidBody>(BodyType::Dynamic));
	player->AddComponent(std::make_shared<PlatformerController>());
	cachPlayer = player;
}

Player::~Player()
{
	cachPlayer = nullptr;
}

void Player::SetAbility(Ability ablilty, UINT value)
{
	switch (ablilty)
	{
	case Player::Ability::HP:
		abilityData._hp += value;
		break;
	case Player::Ability::MP:
		abilityData._mp += value;
		break;
	case Player::Ability::SPEED:
		abilityData._moveSpeed += value;
		break;
	case Player::Ability::STR:
		abilityData._str += value;
		break;
	case Player::Ability::DEX:
		abilityData._dex += value;
		break;
	case Player::Ability::INT:
		abilityData._int += value;
		break;
	case Player::Ability::LUK:
		abilityData._luk += value;
		break;
	case Player::Ability::NONE:
		break;
	default:
		break;
	}
}
