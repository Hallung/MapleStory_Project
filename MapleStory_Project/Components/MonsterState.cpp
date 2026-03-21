#include "stdafx.h"
#include "MonsterState.h"
#include "Animator.h"

void MonsterState::Update()
{
	UpdateHitState();
}

void MonsterState::SetState(Monster::State newState)
{
	if (currentState == newState)	return;

	currentState = newState;
	UpdateAnimation();

	if (newState == Monster::State::DIE)
	{
		GetOwner()->Destroy();
	}
}

void MonsterState::UpdateHitState()
{
	auto animator = GetOwner()->GetComponent<Animator>("Animator");

	if (!animator) return;

	if (currentState == Monster::State::HITTING)
	{
		if (animator->IsFinished())
			SetState(Monster::State::STANDING);
	}
}

void MonsterState::UpdateAnimation()
{
	auto animator = GetOwner()->GetComponent<Animator>("Animator");

	switch (currentState)
	{
	case Monster::State::HITTING:
		animator->Play(L"Hit");
		break;
	case Monster::State::ATTACKING:
		animator->Play(L"Attack");
		break;
	case Monster::State::JUMPING:
		animator->Play(L"Jump");
		break;
	case Monster::State::STANDING:
		animator->Play(L"Stand");
		break;
	case Monster::State::MOVE:
		animator->Play(L"Move");
		break;
	case Monster::State::DIE:
		animator->Play(L"Die");
		break;
	}
}


