#include "stdafx.h"
#include "PlatformerController.h"
#include "Objects/Object.h"
#include "Transform.h"
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

//========================================
// 좌우 이동 적용
// Transform 위치를 deltaTime 기반으로 갱신
//========================================
void PlatformerController::Move(DirectX::SimpleMath::Vector2 dir)
{
	// 수평 입력이 없으면 이동 없음
	if (dir.x == 0.0f) return;

	// 소유 객체의 Transform 획득
	auto transform = GetOwner()->GetComponent<Transform>("Transform");

	// 현재 위치 가져오기
	DirectX::SimpleMath::Vector2 position = transform->GetPosition();

	// 프레임 시간 기반 이동량 계산
	float delta = TimeManager::GetInstance().GetDeltaTime();
	position.x += dir.x * moveSpeed * delta;

	// 위치 적용
	transform->SetPosition(position);
}

void PlatformerController::UpdateAnimation(DirectX::SimpleMath::Vector2 dir)
{
	// TODO : Animatior 컴포넌트 제작 후 작성
}
