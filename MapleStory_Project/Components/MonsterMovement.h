#pragma once
#include "Component.h"

// 몬스터의 실제 몰리 이동을 담당하는 컴포넌트
class MonsterMovement : public Component
{
public:
	MonsterMovement() : Component("MonsterMovement") {}

	void SetMoveSpeed(float speed) { moveSpeed = speed; }	// 이동 속도 설정
	float GetMoveSpeed() const { return moveSpeed; }	// 현재 이동 속도 반환

	void Move(float dir);	// dir 방향으로 이동
	void Stop();	// 이동 정지

private:
	float moveSpeed = 80.0f;	// 몬스터 이동 속도
	float facingDir = 0.0f;		// 현재 바라보는 방향(스프라이트 반전용)
};