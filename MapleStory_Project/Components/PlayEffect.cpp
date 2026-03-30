#include "stdafx.h"
#include "PlayEffect.h"
#include "Animator.h"
#include "Transform.h"
#include "Objects/Object.h"
#include "Objects/Camera.h"

void PlayEffect::Awake()
{
	animator = GetOwner()->GetComponent<Animator>("Animator");
}

void PlayEffect::Play()
{
	auto transform = GetOwner()->GetTransform();

	DirectX::SimpleMath::Vector2 position =
		Camera::main->GetTransform()->GetPosition() +
		DirectX::SimpleMath::Vector2(gWinWidth * 0.5f, gWinHeight * 0.7f);

	transform->SetPosition(position);

	animator->Play(L"Play");
}

void PlayEffect::Stop()
{
	animator->Play(L"Stop");
}
