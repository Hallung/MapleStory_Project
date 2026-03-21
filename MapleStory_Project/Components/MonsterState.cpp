#include "stdafx.h"
#include "MonsterState.h"
#include "Animator.h"

void MonsterState::SetState(Monster::State newState)
{
	if (currentState == newState)	return;

	currentState = newState;

	UpdateAnimation(newState);
}

void MonsterState::UpdateAnimation(Monster::State next)
{
	auto animator = GetOwner()->GetComponent<Animator>("Animator");

	switch (next)
	{
	case Monster::State::STANDING:
		animator->Play(L"Stand");
		break;
	case Monster::State::HITTING:
		animator->Play(L"Hit");
		break;
	case Monster::State::DIE:
		animator->Play(L"Die");
		break;
	}
}
