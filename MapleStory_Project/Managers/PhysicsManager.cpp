#include "stdafx.h"
#include "PhysicsManager.h"
#include "TimeManager.h"

namespace
{
// 물리 고정 시간 (60Hz)
constexpr float fixedTime = 1.0f / 60.0f;
}

PhysicsManager::PhysicsManager() {}

void PhysicsManager::Init()
{
	// 기존 월드가 존재하면 먼저 파괴
	if (b2World_IsValid(worldId)) Destroy();

	// Box2D 월드 정의 생성
	b2WorldDef worldDef = b2DefaultWorldDef();
	// 중력 설정 (-Y축, 아래 방향)
	worldDef.gravity = { 0.0f, -30.0f };
	// 물리 월드 생성
	worldId = b2CreateWorld(&worldDef);
}

void PhysicsManager::Destroy()
{
	// 유효한 월드가 있을 경우 파괴
	if (b2World_IsValid(worldId))
	{
		b2DestroyWorld(worldId);
		worldId = b2_nullWorldId;
	}
}

void PhysicsManager::Update()
{
	// 프레임 deltaTime 가져오기
	float deltaTime = TimeManager::GetInstance().GetDeltaTime();

	// 과도한 프레임 시간 제한 (물리 폭주 방지)
	if (deltaTime > 0.25f) deltaTime = 0.25f;

	// 누적 시간에 프레임 시간 추가
	timeAccumulator += deltaTime;
	
	// 고정 시간 만큼 물리 스텝 반복 수행
	while (timeAccumulator >= fixedTime)
	{
		// 월드가 유효한 경우에만 시뮬레이션 진행
		if (b2World_IsValid(worldId))
		{
			// Box2D 물리 스텝 수행
			// fixedTime : 고정 시뮬레이션 시간
			// stepCount : 서브 스텝 반복 횟수 (정확도 향상)
			b2World_Step(worldId, fixedTime, subStepCount);
		}
		// 사용된 시간 차감
		timeAccumulator -= fixedTime;
	}
}