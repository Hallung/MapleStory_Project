#include "stdafx.h"
#include <chrono>
#include "TimeManager.h"

TimeManager::TimeManager()
{
	// 초기 시간 초기화
	currentTime = Clock::now();
	lastTime = currentTime;
}

void TimeManager::Update()
{
	// 현재 시간 갱신
	currentTime = Clock::now();

	// 이전 프레임과의 시간 차이 계산 (초)
	Duration delta = currentTime - lastTime;
	deltaTime = delta.count();

	// 다음 프레임 계산을 위해 lastTime 갱신
	lastTime = currentTime;

	// 누적 시간 갱신(게임 시작 이후 경과 시간)
	worldTime += deltaTime;

	// fps 계산용 시간/프레임 누적
	fpsTimeElapsed += deltaTime;
	++frameCount;
	// 1초가 경과하면 FPS 갱신
	if (fpsTimeElapsed > 1.0)
	{
		fps = frameCount;
		frameCount = 0;
		fpsTimeElapsed = 0.0;
	}
}

void TimeManager::SetTargetFPS(double fps)
{
	// FPS 제한 해제 (0이하)
	if (fps <= 0.0)
		targetFrameTime = Duration::zero();
	else
		targetFrameTime = Duration(1.0 / fps);	// 목표 프레임 시간 = 1 / FPS(초)
}

void TimeManager::WaitToTargetFrameRate()
{
	// FPS 제한이 없으면 즉시 반환
	if (targetFrameTime.count() <= 0.0) return;

	// 현재 프레임 시작 이후 경과 시간 계산
	TimePoint now = Clock::now();
	Duration elapsed = now - currentTime;

	// 목표 프레임 시간까지 대기
	while (elapsed < targetFrameTime)
	{
		now = Clock::now();
		elapsed = now - currentTime;
	}
}
