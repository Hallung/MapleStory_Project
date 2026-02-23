#include "stdafx.h"
#include "PlatformerController.h"
#include "Objects/Object.h"
#include "RigidBody.h"
#include "Utilities/VirtualKey.h"

//===================================
// 매 프레임 입력 기반 이동 처리
// A/D 키 입력으로 좌우 방향 벡터 생성
// 계산된 방향으로 이동 수행
//===================================
void PlatformerController::Update()
{
	DirectX::SimpleMath::Vector2 dir;
	// 오른쪽 이동 입력
	if (InputManager::GetInstance().GetKeyPress(VK_D)) dir.x += 1.0f;
	// 왼쪽 이동 입력
	if (InputManager::GetInstance().GetKeyPress(VK_A)) dir.x -= 1.0f;

	Move(dir);
}


// 물리 기반 이동
void PlatformerController::Move(DirectX::SimpleMath::Vector2 dir)
{
	// 수평 입력이 없으면 이동 없음
	if (dir.x == 0.0f) return;

	// Object에 부착된 RigidBody 컴포넌트 가져오기
	auto rigidBody = owner->GetComponent<RigidBody>("RigidBody");

	// RigidBody가 존재하고 Box2D Body가 유효한 경우에만 속도 설정
	// (물리 월드에 등록되지 않은 Body는 접근 방지)
	if (rigidBody && b2Body_IsValid(rigidBody->GetBodyId()))
	{
		// 입력 방향(dir)에 이동 속도(moveSpeed)를 곱해 속도 설정
		rigidBody->SetVelocity(dir * moveSpeed);
	}
}

void PlatformerController::UpdateAnimation(DirectX::SimpleMath::Vector2 dir)
{
	// TODO : Animatior 컴포넌트 제작 후 작성
}
