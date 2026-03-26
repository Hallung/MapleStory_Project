#pragma once
#include "Component.h"
#include "Objects/DynamicObjects/Monster.h"

// 몬스터의 행동 상태를 관리하는 컴포넌트
class MonsterState : public Component
{
public:
	MonsterState() : Component("MonsterState") {}

	void Awake() override;

	void Update() override;	// 매 프레임 호출 (상태에 따른 내부 로직 업데이트 수행)
	
	void SetState(Monster::State newState);	// 몬스터 상태 변경
	
	Monster::State GetState() const { return currentState; }	// 현재 상태 반환
	
	void UpdateHitState();	// 피격 상태 종료 검사

	void UpdateAnimation();	// 상태에 맞는 애니메이션 재생
	
private:
	Monster::State currentState = Monster::State::STANDING;	// 현재 몬스터 상태

private:
	std::shared_ptr<class Animator> animator;
};