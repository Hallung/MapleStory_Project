#include "stdafx.h"
#include "TileMapEditorScene.h"
#include "Utilities/ObjectFactory.h"
#include "Utilities/VirtualKey.h"
#include "Utilities/FileDialog.h"
#include "Utilities/Random.h"
#include "Components/Transform.h"
#include "Components/CameraController.h"
#include "Components/MeshRenderer.h"
#include "Resources/Material.h"
#include "Objects/TileMap.h"
#include "Objects/Camera.h"

void TileMapEditorScene::Init()
{
	// 타일셋 구성 정보
	constexpr UINT tileCols = 10;
	constexpr UINT tileRows = 18;
	constexpr UINT maxTilesInTileset = tileCols * tileRows;

	// 테스트용 맵 크기 (타일셋 크기와 동일하게 생성)
	constexpr UINT mapWidth = maxTilesInTileset;
	constexpr UINT mapHeight = maxTilesInTileset;

	// Instancing 기반 TileMap 생성
	tileMap = std::make_shared<TileMap>(mapWidth, mapHeight, 64.0f, L"_Textures/Map/tiles.jpg", tileCols, tileRows);

	// 현재 선택된 타일을 시각적으로 보여주기 위한 반투명 사각형
	// 이후 Tile Palette 기능 추가 시 선택 타일 미리보기로 사용할 예정
	cursorObject = ObjectFactory::CreateSprite(DirectX::SimpleMath::Vector2(gWinWidth * 0.5f, gWinHeight * 0.5f), { 64, 64 }, 0.0f, L"_Textures/Map/tiles.jpg");
	cursorObject->GetComponent<MeshRenderer>("MeshRenderer")->GetMaterial()->SetColor({ 1.0f, 1.0f, 1.0f, 0.5f });
	AddObject(cursorObject);

	Camera::main->AddComponent(std::make_shared<CameraController>());
}

void TileMapEditorScene::Destroy()
{
	objects.clear();
	tileMap = nullptr;
	cursorObject = nullptr;
}

void TileMapEditorScene::Update()
{
	__super::Update();

	// 마우스의 화면 좌표 가져오기
	DirectX::SimpleMath::Vector2 mouseScreenPos = InputManager::GetInstance().GetMousePos();

	// 화면 좌표 -> 월드 좌표 변환 (카메라 기준)
	if (Camera::main)
		mouseWorldPos = Camera::main->ScreenToWorld(mouseScreenPos);

	// 월드 좌표 -> 그리드 인덱스로 변환
	currentGridIndex = tileMap->WorldToGrid(mouseWorldPos);

	// 다시 그리드 -> 월드 중앙 좌표로 변환 (스냅 효과)
	DirectX::SimpleMath::Vector2 snappedWorldPos = tileMap->GridToWorld((int)currentGridIndex.x, (int)currentGridIndex.y);
	
	// 커서를 해당 타일 중앙으로 이동
	cursorObject->GetTransform()->SetPosition(snappedWorldPos);

	// ImGui UI 위에 마우스가 올라가 있는 경우 타일맵 편집 입력이 동시에 발생하지 않도록 마우스 입력을 차단
	if (ImGuiManager::GetInstance().WantCaptureMouse() == false)
	{
		// 마우스 눌렀을 때 한번만 랜덤 생성
		if (InputManager::GetInstance().GetKeyDown(VK_LBUTTON))
			paintTileIndex = Random::Range(0, 179);

		// 좌클릭 시 현재 그리드 위치에 타일 생성 (textureIndex = paintTileIndex)
		if (InputManager::GetInstance().GetKeyPress(VK_LBUTTON))
			tileMap->SetTile((int)currentGridIndex.x, (int)currentGridIndex.y, paintTileIndex);

		// 우클릭 시 타일 제거 (textureIndex = -1)
		if (InputManager::GetInstance().GetKeyPress(VK_RBUTTON))
			tileMap->SetTile((int)currentGridIndex.x, (int)currentGridIndex.y, -1);
	}

	// TileMap 좌표 변환 확인용 디버그 창
	ImGui::Begin("TileMap Editor Debug");
	ImGui::Text("Mouse Screen : %.1f, %.1f", mouseScreenPos.x, mouseScreenPos.y);
	ImGui::Text("Mouse World : %.1f, %.1f", mouseWorldPos.x, mouseWorldPos.y);

	ImGui::Separator();

	// 유효 범위 안이면 초록색, 아니면 빨간색 표시
	bool bValid = tileMap->IsValidGrid((int)currentGridIndex.x, (int)currentGridIndex.y);
	if (bValid)
		ImGui::TextColored(ImVec4(0, 1, 0, 1), "Grid Index  : [%d, %d]", (int)currentGridIndex.x, (int)currentGridIndex.y);
	else
		ImGui::TextColored(ImVec4(1, 0, 0, 1), "Grid Index  : [%d, %d] (Out of Bounds)", (int)currentGridIndex.x, (int)currentGridIndex.y);

	ImGui::Separator();

	// Map 저장 버튼
	if (ImGui::Button("Save Map"))
	{
		// Windows 파일 저장 다이얼로그 실행 (XML 확장자만 보이도록 필터 적용)
		std::wstring path = SaveFileDialog(L"XML Files (*.xml)\0*.xml\0");

		// 사용자가 경로를 선택한 경우에만 저장 실행
		if (!path.empty())
			tileMap->Save(path);
	}

	ImGui::SameLine();

	// Map 불러오기 버튼
	if (ImGui::Button("Load Map"))
	{
		// Windows 파일 열기 다이얼로그 실행 (XML 파일만 선택 가능하도록 필터 적용)
		std::wstring path = OpenFileDialog(L"XML Files (*.xml)\0*.xml\0");

		// 경로가 선택되면 TileMap 로드
		if (!path.empty())
			tileMap->Load(path);
	}

	ImGui::End();
}

void TileMapEditorScene::Render()
{
	__super::Render();

	// TileMap Instancing 렌더링
	if (tileMap)
		tileMap->Render();
}
