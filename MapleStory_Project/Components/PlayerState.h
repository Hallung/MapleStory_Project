#pragma once
#include "Component.h"
#include "Objects/DynamicObjects/Player.h"

class PlayerState : public Component
{
public:
	PlayerState() : Component("PlayerState") {}

	void SetState(Player::State newState);
	Player::State GetState() const { return currentState; }

private:
	Player::State currentState = Player::State::STANDING;
};