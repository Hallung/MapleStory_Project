#include "stdafx.h"
#include "Scarecrow.h"
#include "Utilities/ObjectFactory.h"
#include "Components/MeshRenderer.h"
#include "Components/RigidBody.h"
#include "Components/BoxCollider.h"
#include "Components/Animator.h"
#include "Resources/Material.h"

Scarecrow::Scarecrow(DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, float rotation, const std::wstring& texturePath, BodyType bodyType, const std::string& name)
	: Monster(texturePath, bodyType, name)	// Monster 기본 초기화
{
	// ObjectFactory를 통해 스프라이트 기반 Object 생성
	monster = ObjectFactory::CreateSprite(
		position,
		scale,
		rotation,
		texturePath
	);

	// 픽셀 아트 선명도 유지 (Linear -> Point Sampling)
	monster->GetComponent<MeshRenderer>("MeshRenderer")->GetMaterial()->SetPointSampler(true);
	
	// RigidBody 추가 (물리 설정)
	auto monsterRB = std::make_shared<RigidBody>();
	// 몬스터가 충돌 시 넘어짖 않도록 회전 고정
	monsterRB->SetFixedRotation(true);
	monster->AddComponent(monsterRB);

	// Collider 설정 (충돌 영역)
	auto monsterCol = std::make_shared<BoxCollider>();
	// 실제 충돌 판정 크기 설정 (스프라이트 크기와 충돌 크기를 분리)
	monsterCol->SetColliderScale(DirectX::SimpleMath::Vector2(70.0f, 80.0f));
	// 충돌 레이어 : Monster
	monsterCol->SetCollisionLayer(CollisionLayer::Monster);
	// 충돌 대상 : Player, Ground
	monsterCol->SetCollisionMask(CollisionLayer::Player | CollisionLayer::Ground);
	monster->AddComponent(monsterCol);

	// Animator 추가 (애니메이션 시스템)
	auto monsterAni = std::make_shared<Animator>();
	// XML 애니메이션 데이터 로드
	monsterAni->Load(L"_Animations/Scarecrow.xml");
	// 기본 상태 : Stand 애니메이션 재생
	monsterAni->Play(L"Stand");
	monster->AddComponent(monsterAni);
}
