#include "stdafx.h"
#include "PlayerState.h"

// 플레이어 상태 변경 함수
void PlayerState::SetState(Player::State newState)
{
	// 동일 상태로의 변경 방지
	if (currentState == newState) return;

	// 상태 갱신
	currentState = newState;
}