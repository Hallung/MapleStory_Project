#include "stdafx.h"
#include "ClearEffect.h"
#include "Objects/Object.h"
#include "Components/Animator.h"
#include "Components/PlayEffect.h"
#include "Utilities/ObjectFactory.h"

// Sprite + Animator + PlayerEffect 설정
ClearEffect::ClearEffect(DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, float rotation, const std::wstring& texturePath, BodyType bodyType, const std::string& name)
	: texturePath(texturePath), bodyType(bodyType), name(name)
{
	// Sprite 생성
	clearEffect = ObjectFactory::CreateSprite(
		DirectX::SimpleMath::Vector2(position),
		DirectX::SimpleMath::Vector2(scale),
		rotation,
		texturePath
	);

	// Animator 컴포넌트 추가 후 클리어 애니메이션 로드
	auto animator = std::make_shared<Animator>();
	clearEffect->AddComponent(animator);
	animator->Load(L"_Animations/Clear.xml");
	animator->Play(L"Stop");	// 초기 상태는 Stop

	// PlayEffect 컴포넌트 추가
	clearEffect->AddComponent(std::make_shared<PlayEffect>());
}
