#include "stdafx.h"
#include "Scarecrow.h"
#include "Utilities/ObjectFactory.h"
#include "Components/MeshRenderer.h"
#include "Components/RigidBody.h"
#include "Components/BoxCollider.h"
#include "Components/Animator.h"
#include "Components/MonsterAbility.h"
#include "Components/MonsterState.h"
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
	// 몬스터가 충돌 시 넘어지지 않도록 회전 고정
	monsterRB->SetFixedRotation(true);
	monster->AddComponent(monsterRB);

	//===============================================
	// 몬스터 Sensor Collider 생성
	// 플레이어와의 충돌 감지를 위한 Sensor Collider
	// 물리 반응 없이 겹침만 감지하여 이벤트 처리에 사용
	//===============================================
	auto monsterSensorCol = std::make_shared<BoxCollider>("SensorCollider");

	// Sensor 충돌 영역 크기 설정(플레이어 감지를 위한 범위)
	monsterSensorCol->SetColliderScale(DirectX::SimpleMath::Vector2(60.0f, 80.0f));
	// 물리 충돌이 아닌 Sensor 이벤트로 동작하도록 설정
	monsterSensorCol->SetIsSensor(true);
	// Monster 레이어로 설정
	monsterSensorCol->SetCollisionLayer(CollisionLayer::Monster);
	// Player 레이어와만 충돌 감지하도록 설정
	monsterSensorCol->SetCollisionMask(CollisionLayer::Player | CollisionLayer::Weapon);
	// 몬스터 Object에 Sensor Collider 추가
	monster->AddComponent(monsterSensorCol);

	// Collider 설정 (충돌 영역)
	auto monsterCol = std::make_shared<BoxCollider>();
	// 실제 충돌 판정 크기 설정 
	// (스프라이트 크기와 충돌 크기를 분리, X축 크기를 줄여 실제 물리 충돌 영역을 축소)
	monsterCol->SetColliderScale(DirectX::SimpleMath::Vector2(10.0f, 86.0f));
	// 충돌 레이어 : Monster
	monsterCol->SetCollisionLayer(CollisionLayer::Monster);
	// 충돌 대상 : Player, Ground
	monsterCol->SetCollisionMask(static_cast<uint32_t>(CollisionLayer::Ground));
	monster->AddComponent(monsterCol);

	// State를 관리하는 컴포넌트 추가
	monster->AddComponent(std::make_shared<MonsterState>());
	// Ability를 관리하는 컴포넌트 추가
	monster->AddComponent(std::make_shared<MonsterAbility>());

	// Animator 추가 (애니메이션 시스템)
	auto monsterAni = std::make_shared<Animator>();
	// XML 애니메이션 데이터 로드
	monsterAni->Load(L"_Animations/Scarecrow.xml");
	// 기본 상태 : Stand 애니메이션 재생
	monsterAni->Play(L"Stand");
	monster->AddComponent(monsterAni);
}
