#include "stdafx.h"
#include "MonsterAggro.h"
#include "Objects/Object.h"

// 어그로 시간 감소 처리
void MonsterAggro::Update()
{
	if (!_target) return;

	_currentTimer -= TimeManager::GetInstance().GetDeltaTime();

	// 시간이 끝나면 타겟 해제
	if (_currentTimer <= 0.0f)
		ClearAggro();
}

// 공격자를 타겟으로 설정
void MonsterAggro::SetAggro(Object* target)
{
	if (!target) return;

	_target = target->GetTransform();
	_currentTimer = _aggroTime;	// 어그로 시간 리셋
}

// 타겟 제거 (추격 종료)
void MonsterAggro::ClearAggro()
{
	_target = nullptr;
}
