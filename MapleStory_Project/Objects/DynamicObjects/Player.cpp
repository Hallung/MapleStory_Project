#include "stdafx.h"
#include "Player.h"
#include "Resources/Material.h"
#include "Components/Animator.h"
#include "Components/MeshRenderer.h"
#include "Components/BoxCollider.h"
#include "Components/HitEvents.h"
#include "Components/PlatformerController.h"
#include "Components/Transform.h"
#include "Components/PlayerState.h"
#include "Components/PlayerAbility.h"
#include "Utilities/ObjectFactory.h"

namespace
{
constexpr float scaleX = 35.0f;
constexpr float scaleY = 70.0f;
constexpr DirectX::SimpleMath::Vector2 offset = { 30.0f, 0.0f };
}

Player::Player(DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, float rotation, const std::wstring& texturePath, BodyType bodyType, const std::string& name)
	: bodyType(bodyType), texturePath(texturePath)
{
	// Sprite Objet 생성 (렌더 가능한 기본 Player 형태)
	player = ObjectFactory::CreateSprite(
			DirectX::SimpleMath::Vector2(gWinWidth * 0.5f, gWinHeight * 0.5f),
			DirectX::SimpleMath::Vector2(scale),
			rotation,
			texturePath
		);
	// PointSampler 설정
	player->GetComponent<MeshRenderer>("MeshRenderer")->GetMaterial()->SetPointSampler(true);
	// 물리 바디 추가 (이동/충돌 처리)
	auto playerRb = std::make_shared<RigidBody>(bodyType);

	// FixedRotation 설정으로 캐릭터 회전 방지
	playerRb->SetFixedRotation(true);
	player->AddComponent(playerRb);

	// 바디에 맞춰서 쉐이프 추가
	auto playerCol = std::make_shared<BoxCollider>("PlayerCollider");

	//===========================================
	// Player Collider 크기 수동 설정
	// Sprite 이미지 크기와 별개로 Collider 크기를 설정할 수 있도록 분리
	// 이를 통해 렌더링 크기와 충돌 영역을 독립적으로 제어 가능
	//===========================================
	playerCol->SetColliderScale(DirectX::SimpleMath::Vector2(scaleX, scaleY));
	// Player 객체를 Player Collision Layer에 설정
	playerCol->SetCollisionLayer(CollisionLayer::Player);

	//=================================================
	// Player가 충돌할 수 있는 레이어 설정
	// Ground와 Monster 레이어와만 충돌하도록 마스크 지정
	//=================================================
	playerCol->SetCollisionMask(CollisionLayer::Ground | CollisionLayer::Monster);

	// Player Object에 Collider 컴포넌트 추가
	player->AddComponent(playerCol);

	// 공격 가능 범위 지정
	auto playerAttackCol = std::make_shared <BoxCollider>("AttackCollider");
	playerAttackCol->SetColliderScale(DirectX::SimpleMath::Vector2(scaleX * 0.5f, scaleY));
	// Sensor로 만들어서 물리적 연산x, 신호만 보내도록 설정
	playerAttackCol->SetIsSensor(true);
	//=========================================
	// Collision Layer를 Weapon으로 설정
	// Monster 레이어와만 충돌하도록 마스크 지정
	//=========================================
	playerAttackCol->SetCollisionLayer(CollisionLayer::Weapon);
	playerAttackCol->SetCollisionMask(static_cast<uint32_t>(CollisionLayer::Monster));
	// Body에서 일정범위 떨어진 위치에 Shape 생성을 위해 offset 설정
	playerAttackCol->SetOffsetData(offset);
	// Player Object에 AttackCollider 컴포넌트 추가
	player->AddComponent(playerAttackCol);

	// 충돌 이벤트를 확인하는 HitEvents 추가
	auto playerHitEvents = std::make_shared<HitEvents>();
	player->AddComponent(playerHitEvents);

	// State를 관리하는 컴포넌트 추가
	player->AddComponent(std::make_shared<PlayerState>());

	// Ability를 관리하는 컴포넌트 추가
	player->AddComponent(std::make_shared<PlayerAbility>());

	// 플랫폼 이동 컨트롤러 추가
	player->AddComponent(std::make_shared<PlatformerController>());

	// 플레이어 애니메이션 추가
	auto playerAnimator = std::make_shared<Animator>();
	player->AddComponent(playerAnimator);
	// 로드할 .xml 설정
	playerAnimator->Load(L"_Animations/Player1.xml");
	// 초기 상태 설정
	playerAnimator->Play(L"Stand");

	// Player의 충돌 판정을 위한 HitEvents 추가
	player->AddComponent(std::make_shared<HitEvents>());


}