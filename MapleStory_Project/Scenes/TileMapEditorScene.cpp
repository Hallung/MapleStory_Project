#include "stdafx.h"
#include "TileMapEditorScene.h"
#include "Utilities/ObjectFactory.h"
#include "Utilities/VirtualKey.h"
#include "Utilities/FileDialog.h"
#include "Components/Transform.h"
#include "Components/CameraController.h"
#include "Components/MeshRenderer.h"
#include "Components/RigidBody.h"
#include "Components/ChainCollider.h"
#include "Resources/Material.h"
#include "Resources/Texture.h"
#include "Objects/TileMap.h"
#include "Objects/Camera.h"

#include "Objects/DynamicObjects/Player.h"

namespace
{
// 타일셋 구성 정보
constexpr UINT tileCols = 4;
constexpr UINT tileRows = 4;
constexpr UINT maxTilesInTileset = tileCols * tileRows;

// 테스트용 맵 크기
constexpr UINT mapWidth = 30;
constexpr UINT mapHeight = 40;
}

// 테스트용 플레이어 정보 (삭제 예정)
namespace
{
constexpr DirectX::SimpleMath::Vector2 scale = { 120.0f, 120.0f };
constexpr float offsetPos = 200.0f;
constexpr float rotation = 0.0f;
constexpr float halfValue = 0.5f;
}

void TileMapEditorScene::Init()
{
	//Scene 타입을 Editor로 변경
	currentSceneType = SceneType::Editor;

	// Instancing 기반 TileMap 생성
	tileMap = std::make_shared<TileMap>(mapWidth, mapHeight, 64.0f, L"_Textures/Map/MapleTile.png", tileCols, tileRows);

	// 현재 선택된 타일을 시각적으로 보여주기 위한 반투명 사각형
	cursorObject = ObjectFactory::CreateSprite(DirectX::SimpleMath::Vector2(gWinWidth * 0.5f, gWinHeight * 0.5f), { 64, 64 }, 0.0f, L"_Textures/Map/MapleTile.png");
	cursorObjectMaterial = cursorObject->GetComponent<MeshRenderer>("MeshRenderer")->GetMaterial(); // 커서 오브젝트의 Material 참조
	cursorObjectMaterial->SetColor({ 1.0f, 1.0f, 1.0f, 0.5f }); // 반투명 표시
	cursorObjectMaterial->SetAtlasGrid(tileCols, tileRows); // 타일셋 Atlas 정보 설정
	cursorObjectMaterial->SetAtlasIndex(paintTileIndex); // 현재 선택된 타일 표시
	AddObject(cursorObject);

	Camera::main->AddComponent(std::make_shared<CameraController>());
}

void TileMapEditorScene::Destroy()
{
	objects.clear();
	tileMap = nullptr;
	cursorObject = nullptr;
	chainPoints.clear();
	chainObjects.clear();
	player = nullptr;
	worldPlayer = nullptr;
	Camera::main->RemoveComponent<CameraController>();
}

void TileMapEditorScene::Update()
{
	__super::Update();

	// 마우스 입력을 처리하여 타일맵 편집 (그리드 계산, 커서 스냅, 타일 페인팅)
	HandleMouseInput();
	// TileMap Editor UI 및 디버그 창 렌더링
	DrawEditorUI();
	// 체인 생성 전, 입력된 점들을 화면에 미리보기로 렌더링
	DrawChainPreview();

	// 테스트용 플레이어 생성 (삭제 예정)
	PhysicsManager::GetInstance().Update();

	if (InputManager::GetInstance().GetKeyDown(VK_P))
	{
		if (player == nullptr)
		{
			// 화면 중앙 위치에 Player 객체 생성
			player = std::make_shared<Player>(
				DirectX::SimpleMath::Vector2(gWinWidth * halfValue, gWinHeight * halfValue),
				DirectX::SimpleMath::Vector2(scale),
				rotation
			);
			// Player가 관리하는 실제 게임 Object를 씬에 등록
			AddObject(player->GetPlayer());

			worldPlayer = player->GetPlayer();
		}
	}
}

void TileMapEditorScene::Render()
{
	// TileMap Instancing 렌더링
	if (tileMap)
		tileMap->Render();

	__super::Render();
}

void TileMapEditorScene::HandleMouseInput()
{
	// 현재 선택된 타일 인덱스를 커서에 반영
	cursorObjectMaterial->SetAtlasIndex(paintTileIndex);
	// 마우스의 화면 좌표 가져오기
	mouseScreenPos = InputManager::GetInstance().GetMousePos();

	// 화면 좌표 -> 월드 좌표 변환 (카메라 기준)
	if (Camera::main)
		mouseWorldPos = Camera::main->ScreenToWorld(mouseScreenPos);

	// ImGui UI 위에 마우스가 올라가 있는 경우 타일맵 편집 입력이 동시에 발생하지 않도록 마우스 입력을 차단
	if (ImGuiManager::GetInstance().WantCaptureMouse() == false)
	{
		// 현재 모드에 따라 입력 처리 분기
		if (currentMode == EditorMode::Tile)
		{
			cursorObjectMaterial->SetColor({ 1.0f, 1.0f, 1.0f, 0.5f }); // 반투명 표시
			HandleTileInput();
		}
		else if (currentMode == EditorMode::Chain)
		{
			cursorObjectMaterial->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f }); // 투명 표시
			HandleChainInput();
		}
	}
}

void TileMapEditorScene::HandleTileInput()
{
	// 월드 좌표 -> 그리드 인덱스로 변환
	currentGridIndex = tileMap->WorldToGrid(mouseWorldPos);

	// 다시 그리드 -> 월드 중앙 좌표로 변환 (스냅 효과)
	DirectX::SimpleMath::Vector2 snappedWorldPos = tileMap->GridToWorld((int)currentGridIndex.x, (int)currentGridIndex.y);

	// 커서를 해당 타일 중앙으로 이동
	cursorObject->GetTransform()->SetPosition(snappedWorldPos);

	// 좌클릭 시 현재 그리드 위치에 타일 생성 (textureIndex = paintTileIndex)
	if (InputManager::GetInstance().GetKeyPress(VK_LBUTTON))
		tileMap->SetTile((int)currentGridIndex.x, (int)currentGridIndex.y, paintTileIndex);

	// 우클릭 시 타일 제거 (textureIndex = -1)
	if (InputManager::GetInstance().GetKeyPress(VK_RBUTTON))
		tileMap->SetTile((int)currentGridIndex.x, (int)currentGridIndex.y, -1);
}

void TileMapEditorScene::HandleChainInput()
{
	// 좌클릭 시 점 추가
	if (InputManager::GetInstance().GetKeyDown(VK_LBUTTON))
	{
		chainPoints.push_back(mouseWorldPos);
	}

	// 우클릭 시 마지막 점 삭제
	if (InputManager::GetInstance().GetKeyDown(VK_RBUTTON))
	{
		if (!chainPoints.empty())
			chainPoints.pop_back();
	}

	// Space로 ChainObject 생성
	if (InputManager::GetInstance().GetKeyDown(VK_SPACE))
	{
		CreateChainObject();
	}

	// BackSpace로 ChainPoints 비우기
	if (InputManager::GetInstance().GetKeyDown(VK_BACK))
	{
		chainPoints.clear();
	}
}

void TileMapEditorScene::CreateChainObject()
{
	// 최소 2개 이상의 점이 있어야 체인 생성 가능
	if (chainPoints.size() < 2) return;

	std::vector<DirectX::SimpleMath::Vector2> finalPoints;

	{
		// 시작 지점 보정용 포인트 추가 (첫 번째 점 기준 외곽으로 확장)
		auto dir = chainPoints[0] - chainPoints[1];
		dir.Normalize();

		auto start = chainPoints[0] + dir * 10.0f; // 보정 길이
		finalPoints.push_back(start);
	}

	// 실제 입력된 점들 추가
	for (auto& p : chainPoints)
	{
		finalPoints.push_back(p);
	}

	{
		// 끝 지점 보정용 포인트 추가 (마지막 점 기준 외곽으로 확장)
		int last = chainPoints.size() - 1;
		auto dir = chainPoints[last] - chainPoints[last - 1];
		dir.Normalize();

		auto end = chainPoints[last] + dir * 10.0f; // 보정 길이
		finalPoints.push_back(end);
	}

	// Chain 라인 렌더링용 오브젝트 생성
	auto obj = ObjectFactory::CreateChainLine(finalPoints);
	// 정적인 물리 바디 추가
	obj->AddComponent(std::make_shared<RigidBody>(BodyType::Static));

	// ChainCollider 레이어 설정
	auto chain = std::make_shared<ChainCollider>(finalPoints);
	chain->SetCollisionLayer(CollisionLayer::Ground);
	chain->SetCollisionMask(
		CollisionLayer::Player |
		CollisionLayer::Bullet |
		CollisionLayer::Monster |
		CollisionLayer::Raycast
	);

	// ChainCollider 추가
	obj->AddComponent(chain);

	// Scene에 추가 및 관리 리스트에 저장
	AddObject(obj);
	chainObjects.push_back(obj);

	// 입력된 점 초기화
	chainPoints.clear();
}

void TileMapEditorScene::DrawChainPreview()
{
	// 입력된 점들을 선으로 연결하여 미리보기 렌더링
	if (chainPoints.size() >= 2)
	{
		auto drawList = ImGui::GetBackgroundDrawList();

		for (size_t i = 0; i < chainPoints.size() - 1; ++i)
		{
			// 월드 좌표 -> 스크린 좌표 변환
			auto screen0 = Camera::main->WorldToScreen(chainPoints[i]);
			auto screen1 = Camera::main->WorldToScreen(chainPoints[i + 1]);

			// ImGui 좌표계 보정 (Y축 반전)
			screen0.y = gWinHeight - screen0.y;
			screen1.y = gWinHeight - screen1.y;

			ImVec2 p0 = ImVec2(screen0.x, screen0.y);
			ImVec2 p1 = ImVec2(screen1.x, screen1.y);

			// 점들 사이를 선으로 연결
			drawList->AddLine(p0, p1, IM_COL32(0, 255, 0, 255), 2.0f);
		}
	}

	// 마지막 점과 현재 마우스를 연결하여 다음 선 미리보기
	if (!chainPoints.empty())
	{
		auto last = Camera::main->WorldToScreen(chainPoints.back());
		auto mouse = InputManager::GetInstance().GetMousePos();

		last.y = gWinHeight - last.y;
		mouse.y = gWinHeight - mouse.y;

		ImGui::GetBackgroundDrawList()->AddLine(
			ImVec2(last.x, last.y),
			ImVec2(mouse.x, mouse.y),
			IM_COL32(255, 255, 0, 255),
			2.0f
		);
	}
}

void TileMapEditorScene::DrawEditorUI()
{
	ImGui::Begin("TileMap Editor Debug");

	// Map 저장 버튼
	if (ImGui::Button("Save Map"))
	{
		// Windows 파일 저장 다이얼로그 실행 (XML 확장자만 보이도록 필터 적용)
		std::wstring path = SaveFileDialog(L"XML Files (*.xml)\0*.xml\0");

		// 사용자가 경로를 선택한 경우에만 저장 실행
		if (!path.empty())
		{
			// 현재 chainObjects 에 있는 Chain 정보 저장
			tileMap->SetChainData(chainObjects);
			tileMap->Save(path);
		}
	}

	ImGui::SameLine();

	// Map 불러오기 버튼
	if (ImGui::Button("Load Map"))
	{
		// Windows 파일 열기 다이얼로그 실행 (XML 파일만 선택 가능하도록 필터 적용)
		std::wstring path = OpenFileDialog(L"XML Files (*.xml)\0*.xml\0");

		// 경로가 선택되면 TileMap 로드
		if (!path.empty())
		{
			// 현재 chainObjects 에 있는 Chain 정보 정리
			chainObjects.clear();
			tileMap->Load(path, this);
		}
	}

	ImGui::Separator();

	// 편집 모드 선택 (Tile / Chain)
	ImGui::Text("Edit Mode");

	// 타일 배치 모드
	if (ImGui::RadioButton("Tile", currentMode == EditorMode::Tile))
		currentMode = EditorMode::Tile;

	ImGui::SameLine();

	// 체인 배치 모드
	if (ImGui::RadioButton("Chain", currentMode == EditorMode::Chain))
		currentMode = EditorMode::Chain;

	ImGui::Separator();

	// TileMap 좌표 변환 확인용 디버그 창
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

	if (currentMode == EditorMode::Tile)
	{
		// 타일 선택 창
		ImGui::Text("Tile Palette");

		// 현재 선택된 타일 미리보기
		ImGui::Text("Selected Tile");

		// 타일 팔레트에 사용할 텍스처 가져오기
		auto texture = cursorObjectMaterial->GetTexture();
		if (!texture) return;

		// ImGui ImageButton에서 사용할 ShaderResourceView 포인터
		auto srv = texture->GetSRV().Get();

		// 현재 선택된 타일 인덱스를 Atlas 그리드 좌표로 변환
		UINT x = paintTileIndex % tileCols;
		UINT y = paintTileIndex / tileCols;

		// Atlas에서 타일 하나가 차지하는 UV 크기 계산
		float uvWidth = 1.0f / tileCols;
		float uvHeight = 1.0f / tileRows;

		// 현재 선택된 타일의 UV 시작/끝 좌표
		ImVec2 uv0 = { x * uvWidth, y * uvHeight };
		ImVec2 uv1 = { uv0.x + uvWidth, uv0.y + uvHeight };

		// Atlas의 특정 UV 영역을 잘라서 Image 출력
		ImGui::Image((ImTextureID)srv, ImVec2(80, 80), uv0, uv1);

		// 현재 선택된 타일 인덱스
		ImGui::Text("Index : %d", paintTileIndex);

		ImGui::Separator();

		// 타일셋에 포함된 모든 타일을 ImageButton 형태로 출력
		for (UINT i = 0; i < maxTilesInTileset; ++i)
		{
			// 현재 타일 인덱스를 Atlas 그리드 좌표로 변환
			UINT x = i % tileCols;
			UINT y = i / tileCols;

			// Atlas에서 타일 하나가 차지하는 UV 크기 계산
			float uvWidth = 1.0f / tileCols;
			float uvHeight = 1.0f / tileRows;

			// 현재 타일의 UV 시작/끝 좌표
			ImVec2 uv0 = { x * uvWidth, y * uvHeight };
			ImVec2 uv1 = { uv0.x + uvWidth, uv0.y + uvHeight };

			// 현재 선택된 타일인지 여부
			bool selected = (paintTileIndex == i);

			// 선택된 타일은 버튼 색상과 테두리를 변경하여 강조 표시
			if (selected)
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 1, 0, 1));
				ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2.0f);
			}

			// 동일한 버튼 ID 충돌을 방지하기 위해 인덱스를 ID 스택에 추가
			ImGui::PushID(i);

			// Atlas의 특정 UV 영역을 잘라서 ImageButton으로 출력
			if (ImGui::ImageButton("Tile", (ImTextureID)srv, ImVec2(40, 40), uv0, uv1))
			{
				// 클릭 시 해당 타일을 현재 페인트 타일로 선택
				paintTileIndex = i;
			}

			ImGui::PopID();

			// 선택 강조 스타일 복원
			if (selected)
			{
				ImGui::PopStyleColor();
				ImGui::PopStyleVar();
			}

			// 한 줄에 tileCols 개씩 배치
			if ((i + 1) % tileCols != 0)
				ImGui::SameLine();
		}
	}

	else if (currentMode == EditorMode::Chain)
	{
		// 체인 생성 모드 안내 UI
		ImGui::Text("Chain Mode");
		ImGui::Text("Left Click : Add Point");
		ImGui::Text("Right Click : Remove Point");
		ImGui::Text("BackSpace : Clear Point");
		ImGui::Text("Space : Create Chain");
	}

	ImGui::End();
}
