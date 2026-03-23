#pragma once
#include "Component.h"
#include "Objects/DynamicObjects/Player.h"

//===================================================
// 플레이어의 현재 행동 상태(State)를 관리하는 컴포넌트
// 애니메이션, 입력 제한, 행동 로직 분기에 사용
//===================================================
class PlayerState : public Component
{
public:
	PlayerState() : Component("PlayerState") {}

	void SetState(Player::State newState);	// 플레이어 상태 변경
	Player::State GetState() const { return currentState; }	// 현재 플레이어 상태 반환

private:
	// 현재 플레이어 상태 (기본값: STANDING)
	Player::State currentState = Player::State::STANDING;
};