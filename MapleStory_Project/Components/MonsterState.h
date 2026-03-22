#pragma once
#include "Component.h"
#include "Objects/DynamicObjects/Monster.h"

class MonsterState : public Component
{
public:
	MonsterState() : Component("MonsterState") {}

	void Update() override;

	void SetState(Monster::State newState);
	Monster::State GetState() const { return currentState; }

	void UpdateHitState();

	void UpdateAnimation();
	
private:
	Monster::State currentState = Monster::State::STANDING;
};