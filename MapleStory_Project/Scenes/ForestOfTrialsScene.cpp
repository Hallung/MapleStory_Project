#include "stdafx.h"
#include "ForestOfTrialsScene.h"
#include "Objects/Camera.h"
#include "Objects/TileMap.h"
#include "Objects/DynamicObjects/Player.h"
#include "Components/Transform.h"
#include "Components/MeshRenderer.h"
#include "Resources/Material.h"
#include "Utilities/ObjectFactory.h"
#include "Utilities/VirtualKey.h"

namespace
{
// 타일셋 구성 정보
constexpr UINT tileCols = 4;
constexpr UINT tileRows = 4;
constexpr float tileSize = 64.0f;

// 맵 크기
constexpr UINT mapWidth = 30;
constexpr UINT mapHeight = 40;

// 플레이어 정보
constexpr DirectX::SimpleMath::Vector2 scale = { 120.0f, 120.0f };
constexpr float offsetPos = 200.0f;
constexpr float rotation = 0.0f;
constexpr float halfValue = 0.5f;
}

void ForestOfTrialsScene::Init()
{
	isCleared = false;
	clearTime = 0.0;

	float worldMapWidth = mapWidth * tileSize;
	float worldMapHeight = mapHeight * tileSize;

	backGroundImage1 = ObjectFactory::CreateSprite(
		DirectX::SimpleMath::Vector2(worldMapWidth * 0.5f, worldMapHeight * 0.5f),
		DirectX::SimpleMath::Vector2(worldMapWidth, worldMapHeight),
		0.0f,
		L"_Textures/Map/back.0.png"
	);
	backGroundImage1->GetComponent<MeshRenderer>("MeshRenderer")->GetMaterial()->SetColor(DirectX::SimpleMath::Color(1.0f, 1.0f, 1.0f, 0.8f));
	
	backGroundImage2 = ObjectFactory::CreateSprite(
		DirectX::SimpleMath::Vector2(worldMapWidth * 0.5f, worldMapHeight * 0.5f),
		DirectX::SimpleMath::Vector2(worldMapWidth, worldMapHeight),
		0.0f,
		L"_Textures/Map/back.1.png"
	);

	backGroundImage3 = ObjectFactory::CreateSprite(
		DirectX::SimpleMath::Vector2(worldMapWidth * 0.5f, tileSize * 6.0f),
		DirectX::SimpleMath::Vector2(worldMapWidth, worldMapHeight * 0.5f),
		0.0f,
		L"_Textures/Map/back.4.png"
	);
	backGroundImage3->GetComponent<MeshRenderer>("MeshRenderer")->GetMaterial()->SetColor(DirectX::SimpleMath::Color(1.0f, 1.0f, 1.0f, 0.8f));

	tileMap = std::make_shared<TileMap>(mapWidth, mapHeight, tileSize, L"_Textures/Map/MapleTile.png", tileCols, tileRows);
	tileMap->Load(L"_XML/ForestOfTrials.xml", this);

	{
		auto guideImage = ObjectFactory::CreateSprite(
			DirectX::SimpleMath::Vector2(gWinWidth * halfValue, gWinHeight * halfValue),
			DirectX::SimpleMath::Vector2(150.0f, 90.0f), 0.0f,
			L"_Textures/MoveGuide.png"
		);

		AddObject(guideImage);
	}

	{
		auto guideImage = ObjectFactory::CreateSprite(
			DirectX::SimpleMath::Vector2(gWinWidth * halfValue + 200.0f, gWinHeight * halfValue),
			DirectX::SimpleMath::Vector2(130.0f, 50.0f), 0.0f,
			L"_Textures/AttackGuide.png"
		);

		AddObject(guideImage);
	}

	{
		auto guideImage = ObjectFactory::CreateSprite(
			DirectX::SimpleMath::Vector2(gWinWidth + 550.0f, gWinHeight * halfValue + 100.0f),
			DirectX::SimpleMath::Vector2(60.0f, 60.0f), 0.0f,
			L"_Textures/JumpGuide.png"
		);

		AddObject(guideImage);
	}

	auto player = std::make_shared<Player>(
		DirectX::SimpleMath::Vector2(gWinWidth * halfValue, gWinHeight * halfValue),
		DirectX::SimpleMath::Vector2(scale),
		rotation
	);

	AddObject(player->GetPlayer());

	worldPlayer = player->GetPlayer();

	SoundManager::GetInstance().PlayBGM("_Sounds/BGM/FloralLife.wav");
}

void ForestOfTrialsScene::Destroy()
{
	objects.clear();
	worldPlayer = nullptr;
	backGroundImage1 = nullptr;
	backGroundImage2 = nullptr;
	backGroundImage3 = nullptr;
	SoundManager::GetInstance().StopBGM();
}

void ForestOfTrialsScene::Update()
{
	if (!isCleared)
	ImGuiManager::GetInstance().ShowPlayTime();

	if (!isCleared && InputManager::GetInstance().GetKeyDown(VK_F))
	{
		isCleared = true;
		clearTime = TimeManager::GetInstance().GetWorldTime();
	}
	else if (isCleared)
	{
		ImGuiManager::GetInstance().ShowClearTime(clearTime);
	}

	backGroundImage1->Update();
	backGroundImage2->Update();
	backGroundImage3->Update();

	__super::Update();
	PhysicsManager::GetInstance().Update();

	Camera::main->SetPositionWithClamp(worldPlayer->GetTransform()->GetPosition(), tileMap.get());
}

void ForestOfTrialsScene::Render()
{
	backGroundImage1->Render();
	backGroundImage2->Render();
	backGroundImage3->Render();

	tileMap->Render();
	__super::Render();

}
