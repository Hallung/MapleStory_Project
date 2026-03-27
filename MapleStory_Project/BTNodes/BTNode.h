#pragma once
#include "stdafx.h"

//==============================================================
// Behavior Tree 실행 결과 상태
// Success: 노드의 작업이 성공적으로 완료됨
// Failure: 조건 불만족 또는 작업 실패 = 다음 노드 탐색
// Running: 노드에서 아직 작업 진행 중 (다음 프레임에도 계속 실행)
//==============================================================
enum class BTState
{
	Success,
	Failure,
	Running
};

//==================================================
// Behavior Tree의 모든 노드가 상속받는 베이스 클래스
// 각 노드는 Tick()을 통해 매 프레임 실행
//==================================================
class BTNode
{
public:
	virtual ~BTNode() = default;

	// 반환값에 따라 부모 노드(Selector/Sequence)의 흐름을 결정
	virtual BTState Tick(float deltaTime) = 0;
};