#include "stdafx.h"
#include "PlayEffect.h"
#include "Animator.h"
#include "Objects/Object.h"

void PlayEffect::Awake()
{
	animator = GetOwner()->GetComponent<Animator>("Animator");
}

void PlayEffect::Update()
{
	if (InputManager::GetInstance().GetKeyDown(VK_DOWN))
		Play();
}

void PlayEffect::Play()
{
	animator->Play(L"Play");
}
