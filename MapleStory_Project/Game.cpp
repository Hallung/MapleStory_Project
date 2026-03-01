#include "stdafx.h"
#include "Game.h"
#include "Scenes/SceneList.h"
#include "Objects/Camera.h"

Game::Game()
{
	mainCamera = std::make_shared<Camera>(); // Main Camera 생성
}

Game::~Game()
{
	currentScene = nullptr; // 현재 Scene 참조 해제
	sceneList.clear(); // Scene 목록 전체 제거
	mainCamera = nullptr; // Main Camera 참조 해제
	PhysicsManager::GetInstance().Destroy(); // Physics 시스템 리소스 정리
	ImGuiManager::GetInstance().Destroy(); // ImGui 컨텍스트 및 백엔드 종료
}

// 게임 초기화
void Game::Init()
{
	// ImGui 시스템 초기화
	ImGuiManager::GetInstance().Init();

	// SandboxScene 생성 및 Scene 목록에 추가
	sceneList.push_back(std::make_shared<SandboxScene>());

	currentScene = sceneList[0]; // 첫 번째 Scene을 현재 Scene으로 설정
	PhysicsManager::GetInstance().Init(); // Physics 시스템 초기화
	currentScene->Init(); // 현재 Scene 초기화
}

// 게임 업데이트
void Game::Update()
{
	// ImGui 프레임 시작
	ImGuiManager::GetInstance().Update();

	currentScene->Update();
	mainCamera->Update();

#ifdef _DEBUG
	// 디버그 모드에서만 FPS 오버레이 출력
	ImGuiManager::GetInstance().ShowFPSOverlay();
#endif
}

// 게임 렌더링
void Game::Render()
{
	mainCamera->Bind();
	currentScene->Render();

	// ImGui 렌더링
	ImGuiManager::GetInstance().Render();
}

// Scene 전환 함수
void Game::SwitchScene(size_t index)
{
	// 유효하지 않은 인덱스 검사
	if (index >= sceneList.size()) return;

	// 동일 Scene 전환 방지
	if (currentScene == sceneList[index]) return;

	currentScene->Destroy(); // 현재 Scene 종료 처리
	PhysicsManager::GetInstance().Destroy(); // Physics 시스템 리소스 정리

	currentScene = sceneList[index]; // Scene 변경

	PhysicsManager::GetInstance().Init(); // Physics 시스템 초기화
	currentScene->Init(); // 새로운 Scene 초기화
}
