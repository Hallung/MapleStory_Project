#include "stdafx.h"
#include "PlayerState.h"

void PlayerState::SetState(Player::State newState)
{
	if (currentState == newState) return;

	currentState = newState;
}