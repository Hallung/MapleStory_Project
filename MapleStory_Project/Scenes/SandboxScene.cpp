#include "stdafx.h"
#include "SandboxScene.h"
#include "Objects/Object.h"
#include "Objects/DynamicObjects/Player.h"
#include "Components/RigidBody.h"

//=====================================
// Player 생성/배치에 사용하는 내부 상수
// 이 cpp 파일 내부에서만 사용되는 값
// 익명 namespace로 외부 접근 차단
//=====================================
namespace
{
constexpr DirectX::SimpleMath::Vector2 scale = { 77.0f, 77.0f };
constexpr float rotation = 0.0f;
constexpr float halfValue = 0.5f;
}

// Scene 초기화, 테스트용 Object AddObject()로 등록 가능
void SandboxScene::Init()
{
	// 화면 중앙 위치에 Player 객체 생성
	auto player = std::make_shared<Player>(
		DirectX::SimpleMath::Vector2(gWinWidth * halfValue, gWinHeight * halfValue),
		DirectX::SimpleMath::Vector2(scale),
		rotation
		);
	// Player가 관리하는 실제 게임 Object를 씬에 등록
	AddObject(player->GetPlayer());
}

// Scene 종료 처리, Scene이 소유한 Object 목록 정리
void SandboxScene::Destroy()
{
	objects.clear();
}

// Scene의 기본 Object Update 수행, 필요 시 Sandbox 전용 테스트 로직 추가 가능
void SandboxScene::Update()
{
	__super::Update();
	PhysicsManager::GetInstance().Update();
}

// Scene의 기본 Object Render 수행, 필요 시 디버그 렌더링 등 확장 가능
void SandboxScene::Render()
{
	__super::Render();
}
