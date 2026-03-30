#include "stdafx.h"
#include "Portal.h"
#include "Resources/Material.h"
#include "Objects/Object.h"
#include "Components/BoxCollider.h"
#include "Components/Animator.h"
#include "Components/MeshRenderer.h"
#include "Utilities/ObjectFactory.h"
#include "Utilities/CollisionLayer.h"

// Collider 크기 상수
namespace
{
constexpr DirectX::SimpleMath::Vector2 groundColScale = { 10.0f, 10.0f };
constexpr DirectX::SimpleMath::Vector2 sensorColScale = { 100.0f, 130.0f };
}

// Portal 생성자
Portal::Portal(DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, float rotation, const std::wstring& texturePath, BodyType bodyType, const std::string& name)
	: texturePath(texturePath), bodyType(bodyType), name(name)
{
	// Sprite 생성
	portal = ObjectFactory::CreateSprite(
		DirectX::SimpleMath::Vector2(position),
		DirectX::SimpleMath::Vector2(scale),
		rotation,
		texturePath
	);

	// 반투명 효과 적용
	portal->GetComponent<MeshRenderer>("MeshRenderer")
		->GetMaterial()->SetColor(DirectX::SimpleMath::Color(1.0f, 1.0f, 1.0f, 0.95f));

	// Rigidbody 추가 (회전 고정)
	auto portalRb = std::make_shared<RigidBody>();
	portalRb->SetFixedRotation(true);
	portal->AddComponent(portalRb);

	// Sensor Collider 추가 (플레이어 감지용)
	auto portalSensorCol = std::make_shared<BoxCollider>("SensorCollider");
	portalSensorCol->SetColliderScale(sensorColScale);
	portalSensorCol->SetIsSensor(true);		// 충돌은 없고 이벤트만 발생
	portalSensorCol->SetCollisionLayer(CollisionLayer::Portal);
	portalSensorCol->SetCollisionMask(static_cast<uint32_t>(CollisionLayer::Player));
	portal->AddComponent(portalSensorCol);

	// Animator 추가 및 Portal 애니메이션 재생
	auto portalAni = std::make_shared<Animator>();
	portal->AddComponent(portalAni);
	portalAni->Load(L"_Animations/Portal.xml");
	portalAni->Play(L"Portal");
}
