#include "stdafx.h"
#include "Game.h"
#include "Scenes/SceneList.h"
#include "Objects/Camera.h"
#include "Utilities/Random.h"

Game::Game()
{
	mainCamera = std::make_shared<Camera>(); // Main Camera 생성
	
	// 생성된 Main Camera의 실제 주소를 static main 포인터에 등록
	Camera::main = mainCamera.get();
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

	// 랜덤 생성기 초기화 (seed 설정)
	Random::Init();

	// SandboxScene 생성 및 Scene 목록에 추가
	sceneList.push_back(std::make_shared<SandboxScene>());
	
	// TileMapEditorScene 생성 및 Scene 목록에 추가
	sceneList.push_back(std::make_shared<TileMapEditorScene>());

	// TitleScene 생성 및 Scene 목록에 추가
	sceneList.push_back(std::make_shared<TitleScene>());

	// ForestOfTrialsScene 생성 및 Scene 목록에 추가
	sceneList.push_back(std::make_shared<ForestOfTrialsScene>());

	currentScene = sceneList[2]; // Title Scene을 현재 Scene으로 설정
	currentSceneID = SceneID::Title; // currentSceneID를 Title로 설정

	PhysicsManager::GetInstance().Init(); // Physics 시스템 초기화
	SoundManager::GetInstance().Init(); // Sound 시스템 초기화
	currentScene->Init(); // 현재 Scene 초기화
}

// 게임 업데이트
void Game::Update()
{
	// ImGui 프레임 시작
	ImGuiManager::GetInstance().Update();

	// Scene 전환
	if (InputManager::GetInstance().GetKeyDown(VK_F1))
	{
		SwitchScene(0); // SandboxScene
	}
	else if (InputManager::GetInstance().GetKeyDown(VK_F2))
	{
		SwitchScene(1); // TileMapEditorScene
	}
	else if (InputManager::GetInstance().GetKeyDown(VK_F3))
	{
		SwitchScene(2); // TitleScene
	}

	if (currentSceneID == SceneID::Title)
	{
		if (InputManager::GetInstance().GetKeyDown(VK_RETURN))
		{
			SwitchScene(currentScene->GetNextSceneID());
		}
	}

	if (currentSceneID == SceneID::Forest && currentScene->GetRequestSceneChange() == true)
	{
		if (InputManager::GetInstance().GetKeyDown(VK_RETURN))
		{
			SwitchScene(currentScene->GetNextSceneID());
		}
	}

	currentScene->Update();
	mainCamera->Update();

	// Sound 상태 갱신
	SoundManager::GetInstance().Update();

#ifdef _DEBUG
	// 디버그 모드에서만 FPS 오버레이 출력
	ImGuiManager::GetInstance().ShowFPSOverlay();

	// Inspector 창 시작
	// 현재 씬이 OnImGui()를 override 했다면 해당 씬의 디버그 UI가 이 창 안에 출력
	ImGui::Begin("Inspector");
	// 씬별 ImGui 출력 (없으면 기본 빈 창)
	currentScene->OnImGui();
	// Inspector 창 종료
	ImGui::End();
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
	currentSceneID = static_cast<SceneID>(index); // SceneID 값 변경
	currentScene->ResetRequestSceneChange(); // requestSceneChange 변수 초기화

	mainCamera->Reset(); // 메인 카메라 포지션 및 줌 초기화
	PhysicsManager::GetInstance().Init(); // Physics 시스템 초기화
	TimeManager::GetInstance().Reset(); // Time 시스템 초기화
	currentScene->Init(); // 새로운 Scene 초기화
}

void Game::SwitchScene(SceneID id)
{
	SwitchScene(static_cast<size_t>(id));
}
