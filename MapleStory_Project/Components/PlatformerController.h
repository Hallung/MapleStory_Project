#pragma once
#include "Component.h"

//=======================================
// 2D 플렛폼 캐릭터 이동 컨트롤러 컴포넌트
// 좌우 이동입력 처리 및 속도 적용
// 이동 방향에 따른 애니메이션 상태 갱신
//=======================================
class PlatformerController : public Component
{
public:
	//===============================================
	// 기본 이동 속도를 가지는 컨트롤러 생성
	// Component 이름을 "PlatformerController"로 설정
	//===============================================
	PlatformerController(float moveSpeed = 200.0f) : Component("PlatformerController"), moveSpeed(moveSpeed) {}

	// 매 프레임 입력 기반 이동 및 애니메이션 업데이트
	void Update() override;

private:
	// 이동 처리 (방향 벡터(dir)에 이동 속도를 곱해 워치 갱신
	void Move(DirectX::SimpleMath::Vector2 dir);
	
	// 애니메이션 상태 업데이트(이동 방향에 따라 상태 변경)
	void UpdateAnimation(DirectX::SimpleMath::Vector2 dir);

	float moveSpeed = 0.0f;	// 이동 속도
};