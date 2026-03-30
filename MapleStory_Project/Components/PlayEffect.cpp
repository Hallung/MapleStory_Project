#include "stdafx.h"
#include "PlayEffect.h"
#include "Animator.h"
#include "Transform.h"
#include "Objects/Object.h"
#include "Objects/Camera.h"

// Owner의 Animator 컴포넌트 획득
void PlayEffect::Awake()
{
	animator = GetOwner()->GetComponent<Animator>("Animator");
}

// 화면 중앙에 위치시키고 Play 애니메이션 재생
void PlayEffect::Play()
{
	auto transform = GetOwner()->GetTransform();

	// 카메라 기준 화면 위치 계산
	DirectX::SimpleMath::Vector2 position =
		Camera::main->GetTransform()->GetPosition() +
		DirectX::SimpleMath::Vector2(gWinWidth * 0.5f, gWinHeight * 0.6f);

	// Transform 위치 갱신
	transform->SetPosition(position);

	// Play 애니메이션 재생
	animator->Play(L"Play");
}

// Stop 애니메이션 재생
void PlayEffect::Stop()
{
	animator->Play(L"Stop");
}
