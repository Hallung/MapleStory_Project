#pragma once
#include "Component.h"
#include "Collider.h"
#include "BTNodes/BTNode.h"

//=============================================================
// Behavior Tree 노드 간 데이터를 공유하기 위한 Blackboard 구조체
// AI의 "기억 공간" 역할
//=============================================================
struct MonsterBlackboard
{
	// 현재 타겟 (추적 대상)
	Collider* target = nullptr;

	float wanderTimer = 0.0f;	// 배회 상태 타이머
	float idleTimer = 0.0f;		// Idle 상태 유지 시간

	float wanderDirection = 0.0f;	// 배회 이동 방향

	// 배회 상태 정의
	enum class WanderState
	{
		Idle,
		Move
	};

	// 현재 배회 상태
	WanderState wanderState = WanderState::Idle;
};

// 몬스터의 Behavior Tree 실행을 담당하는 AI 컴포넌트
class MonsterAI : public Component
{
public:
	MonsterAI() : Component("MonsterAI") {}

	void Awake() override;
	void Update() override;

	// Blackboard 접근 함수
	MonsterBlackboard& GetBlackboard() { return blackboard; }

private:
	std::unique_ptr<class BTNode> root;	// Behavior Tree 루트 노드

	// 상태 및 피격 이벤트 참조
	std::shared_ptr<class MonsterState> state;
	std::shared_ptr<class HitEvents> hitEvents;

	MonsterBlackboard blackboard;	// AI 공유 데이터
};