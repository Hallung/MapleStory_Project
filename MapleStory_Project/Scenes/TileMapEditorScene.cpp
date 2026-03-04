#include "stdafx.h"
#include "TileMapEditorScene.h"
#include "Utilities/ObjectFactory.h"
#include "Components/Transform.h"
#include "Objects/TileMap.h"
#include "Objects/Camera.h"

void TileMapEditorScene::Init()
{
	// 20x20 크기, 타일 크기 64인 TileMap 생성
	tileMap = std::make_shared<TileMap>(20, 20, 64.0f);

	// 현재 선택된 타일을 시각적으로 보여주기 위한 반투명 사각형
	cursorObject = ObjectFactory::CreateColorRect(DirectX::SimpleMath::Vector2(gWinWidth * 0.5f, gWinHeight * 0.5f), { 64, 64 }, 0.0f, { 1.0f, 0.0f, 0.0f, 0.5f });
	AddObject(cursorObject);

	// 실제 타일이 존재할 경우 화면에 그려줄 테스트용 오브젝트
	// 현재는 하나의 프리팹을 위치만 바꿔가며 무식하게 렌더링 중
	tilePrefab = ObjectFactory::CreateColorRect(DirectX::SimpleMath::Vector2(gWinWidth * 0.5f, gWinHeight * 0.5f), { 64, 64 }, 0.0f, { 0.8f, 0.8f, 0.8f, 1.0f });
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

	// 좌클릭 시 현재 그리드 위치에 타일 생성 (textureIndex = 0)
	if (InputManager::GetInstance().GetKeyPress(VK_LBUTTON))
		tileMap->SetTile((int)currentGridIndex.x, (int)currentGridIndex.y, 0);
	// 우클릭 시 타일 제거 (textureIndex = -1)
	if (InputManager::GetInstance().GetKeyPress(VK_RBUTTON))
		tileMap->SetTile((int)currentGridIndex.x, (int)currentGridIndex.y, -1);

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

	ImGui::End();
}

void TileMapEditorScene::Render()
{
	__super::Render();

	for (UINT y = 0; y < tileMap->GetHeight(); ++y)
	{
		for (UINT x = 0; x < tileMap->GetWidth(); ++x)
		{
			// 타일이 존재하는 경우에만 렌더
			const TileInfo* tile = tileMap->GetTile(x, y);

			if (tile != nullptr && tile->textureIndex >= 0)
			{
				// 해당 그리드의 월드 중앙 좌표 계산
				DirectX::SimpleMath::Vector2 worldPos = tileMap->GridToWorld(x, y);

				// 프리팹 위치 이동
				tilePrefab->GetTransform()->SetPosition(worldPos);
				// Transform 갱신 후 직접 Render 호출 (현재는 구조 테스트 단계라 일단 무식하게 처리)
				tilePrefab->GetTransform()->Update();
				tilePrefab->Render();
			}
		}
	}
}
