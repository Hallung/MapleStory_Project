#pragma once
#include "Component.h"
#include "Transform.h"

//=====================================
// 몬스터의 어그로 관리 컴포넌트
// 일정 시간 동안 공격자를 기억하고 추격
//=====================================
class MonsterAggro : public Component
{
public:
	MonsterAggro() : Component("MonsterAggro") {}

	void Update() override;

	void SetAggro(Object* target);	// 타게 설정 (공격 당했을 때 호출)
	Transform* GetTarget() const { return _target; }	// 현재 타겟 반환
	void ClearAggro();	// 어그로 해제

	bool HasTarget() const { return _target != nullptr; }	// 타겟 존재 여부

private:
	Transform* _target = nullptr;	// 현재 추격 대상

	float _aggroTime = 5.0f;		// 어그로 유지 시간
	float _currentTimer = 0.0f;		// 남은 어그로 시간
};