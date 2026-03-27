#include "stdafx.h"
#include "ClearEffect.h"
#include "Objects/Object.h"
#include "Components/Animator.h"
#include "Components/PlayEffect.h"
#include "Utilities/ObjectFactory.h"

ClearEffect::ClearEffect(DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, float rotation, const std::wstring& texturePath, BodyType bodyType, const std::string& name)
	: texturePath(texturePath), bodyType(bodyType), name(name)
{
	clearEffect = ObjectFactory::CreateSprite(
		DirectX::SimpleMath::Vector2(position),
		DirectX::SimpleMath::Vector2(scale),
		rotation,
		texturePath
	);

	auto animator = std::make_shared<Animator>();
	clearEffect->AddComponent(animator);
	animator->Load(L"_Animations/Clear.xml");
	animator->Play(L"Stop");

	clearEffect->AddComponent(std::make_shared<PlayEffect>());
}
