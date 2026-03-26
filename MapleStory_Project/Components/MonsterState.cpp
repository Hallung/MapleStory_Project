#include "stdafx.h"
#include "MonsterState.h"
#include "Animator.h"

void MonsterState::Update()
{
	UpdateHitState();
}

// 몬스터 상태 변경 처리
void MonsterState::SetState(Monster::State newState)
{
	// 동일 상태 변경 방지
	if (currentState == newState)	return;

	currentState = newState;	// 상태 갱신
	UpdateAnimation();	// 상태 변경 시 애니메이션 동기화

	// 사망 상태 진입 시 객체 제거
	if (newState == Monster::State::DIE)
	{
		// TODO: Scene에서 하고있는 OnDelete를 해당 지점에서 시행 후 
		//		if (animator->IsFinish())를 한 후 Destroy 하도록 변경
		GetOwner()->Destroy();
	}
}

//=====================================================
// 피격 애니메이션 종류 여부 검사
// 피격 애니메이션이 끝나면 자동으로 STANDING 상태로 복귀
//=====================================================
void MonsterState::UpdateHitState()
{
	auto animator = GetOwner()->GetComponent<Animator>("Animator");

	if (!animator) return;

	// 현재 피격 상태라면
	if (currentState == Monster::State::HITTING)
	{
		// 애니메이션 종류 확인
		if (animator->IsFinished())
			SetState(Monster::State::STANDING);
	}
}

// 현재 상태에 맞는 애니메이션 재생
void MonsterState::UpdateAnimation()
{
	auto animator = GetOwner()->GetComponent<Animator>("Animator");

	if (!animator) return;

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
	case Monster::State::CHASE:
		animator->Play(L"Move");
		break;
	case Monster::State::DIE:
		animator->Play(L"Die");
		break;
	}
}


