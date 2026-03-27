#include "stdafx.h"
#include "MonsterAI.h"
#include "MonsterState.h"
#include "HitEvents.h"
#include "BTNodes/Selector.h"
#include "BTNodes/ChasePlayerNode.h"
#include "BTNodes/IdleNode.h"
#include "BTNodes/WanderNode.h"

// Behavior Tree 구성
void MonsterAI::Awake()
{
	state = GetOwner()->GetComponent<MonsterState>("MonsterState");
	hitEvents = GetOwner()->GetComponent<HitEvents>("HitEvents");

	// Selector: Chase -> Wander -> Idle 순으로 우선순위 실행
	auto rootSelector = std::make_unique<Selector>();
	// 플레이어 추격
	rootSelector->AddChild(std::make_unique<ChasePlayerNode>(this));
	// 랜덤 이동
	rootSelector->AddChild(std::make_unique<WanderNode>(this));
	// 대기
	rootSelector->AddChild(std::make_unique<IdleNode>(this));

	root = move(rootSelector);
}

// 매 프레임 Behavior Tree 실행
void MonsterAI::Update()
{
	// 피격 또는 사망 중에는 AI 정지
	if (state->GetState() == Monster::State::HITTING ||
		state->GetState() == Monster::State::DIE)
		return;

	if (root)
		root->Tick(TimeManager::GetInstance().GetDeltaTime());
}
