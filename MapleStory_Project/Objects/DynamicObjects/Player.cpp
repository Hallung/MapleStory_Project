#include "stdafx.h"
#include "Player.h"
#include "Components/PlatformerController.h"
#include "Utilities/ObjectFactory.h"

Player::Player(DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, float rotation, const std::wstring& texturePath, BodyType bodyType, const std::string& name)
	: bodyType(bodyType), texturePath(texturePath)
{
	// Sprite Objet 생성 (렌더 가능한 기본 Player 형태)
	auto player = ObjectFactory::CreateSprite(
			DirectX::SimpleMath::Vector2(gWinWidth * 0.5f, gWinHeight * 0.5f),
			DirectX::SimpleMath::Vector2(scale),
			rotation,
			texturePath
		);
	// 물리 바디 추가 (이동/충돌 처리)
	player->AddComponent(std::make_shared<RigidBody>(bodyType));
	// 플랫폼 이동 컨트롤러 추가
	player->AddComponent(std::make_shared<PlatformerController>());
	// 내부 Player Object 캐싱
	cachPlayer = player;
}

// Player 제거 시 내부 Object 참조 해제
Player::~Player()
{
	cachPlayer = nullptr;
}

//=====================================
// 능력치 증가
// Ability 종류에 따라 해당 스탯 값 누적
//=====================================
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
