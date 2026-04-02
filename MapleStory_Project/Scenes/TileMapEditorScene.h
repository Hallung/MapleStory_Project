#pragma once
#include "Scene.h"

// 에디터에서 사용할 편집 모드
enum class EditorMode
{
	Tile,
	Chain
};

// TileMap 구조 테스트 및 좌표 변환 확인용 Scene
class TileMapEditorScene : public Scene
{
public:
	void Init() override;
	void Destroy() override;
	void Update() override;
	void Render() override;

private:
	// 마우스 입력을 받아 현재 모드에 맞는 편집 로직 실행
	void HandleMouseInput();
	// 타일 배치 모드 입력 처리 (그리드 기반)
	void HandleTileInput();
	// 체인 배치 모드 입력 처리 (자유 좌표 기반)
	void HandleChainInput();
	// 현재 찍은 점들을 기반으로 ChainCollider 오브젝트 생성
	void CreateChainObject();
	// 체인 생성 전, 입력된 점들을 화면에 미리보기로 렌더링
	void DrawChainPreview();
	// TileMap Editor 전용 ImGui UI 렌더링
	void DrawEditorUI();
	// Scene 초기화 함수
	void ResetScene();

private:
	// 테스트용 TileMap
	std::shared_ptr<class TileMap> tileMap;
	// 현재 선택된 타일 위치를 표시하기 위한 커서 오브젝트
	std::shared_ptr<class Object> cursorObject;
	// 커서 오브젝트의 머티리얼
	std::shared_ptr<class Material> cursorObjectMaterial;
	// 타일 인덱스를 저장하는 변수
	UINT paintTileIndex = 0;

	// 현재 마우스가 위치한 그리드 인덱스
	DirectX::SimpleMath::Vector2 currentGridIndex;
	// 마우스의 월드 좌표
	DirectX::SimpleMath::Vector2 mouseWorldPos;
	// 마우스의 화면 좌표
	DirectX::SimpleMath::Vector2 mouseScreenPos;

	// 현재 에디터 모드
	EditorMode currentMode = EditorMode::Tile;
	// 체인 생성 시 사용자가 찍은 점들 (월드 좌표)
	std::vector<DirectX::SimpleMath::Vector2> chainPoints;
	// 생성된 Chain 오브젝트들을 관리하는 컨테이너
	std::vector<std::shared_ptr<Object>> chainObjects;

	std::shared_ptr<Object> worldPlayer;
};