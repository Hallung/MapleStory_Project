#pragma once
#include "Component.h"
#include "Objects/DynamicObjects/Monster.h"

class MonsterState : public Component
{
public:
	MonsterState() : Component("MonaterState") {}

	void SetState(Monster::State newState);
	Monster::State GetState() const { return currentState; }

	void UpdateAnimation(Monster::State next);
	
private:
	Monster::State currentState = Monster::State::STANDING;
};