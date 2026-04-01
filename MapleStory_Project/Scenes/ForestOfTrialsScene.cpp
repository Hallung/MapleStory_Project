#include "stdafx.h"
#include "ForestOfTrialsScene.h"
#include "Objects/Camera.h"
#include "Objects/TileMap.h"
#include "Objects/DynamicObjects/Player.h"
#include "Objects/StaticObjects/Portal.h"
#include "Objects/StaticObjects/ClearEffect.h"
#include "Objects/DynamicObjects/Scarecrow.h"
#include "Objects/DynamicObjects/OrangeMushroom.h"
#include "Components/Transform.h"
#include "Components/MeshRenderer.h"
#include "Components/PlayEffect.h"
#include "Components/PlatformerController.h"
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
constexpr float worldMapWidth = mapWidth * tileSize;
constexpr float worldMapHeight = mapHeight * tileSize;

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

	DrawBackGroundImage();

	tileMap = std::make_shared<TileMap>(mapWidth, mapHeight, tileSize, L"_Textures/Map/MapleTile.png", tileCols, tileRows);
	tileMap->Load(L"_XML/ForestOfTrials.xml", this);

	DrawGuideImage();

	SpawnMonsters();

	auto player = std::make_shared<Player>(
		DirectX::SimpleMath::Vector2(tileSize * 10.0f, tileSize * 4.0f),
		DirectX::SimpleMath::Vector2(scale),
		rotation
	);

	AddObject(player->GetPlayer());

	worldPlayer = player->GetPlayer();

	auto portal = std::make_shared<Portal>(
		DirectX::SimpleMath::Vector2(100.0f, worldMapHeight - tileSize),
		DirectX::SimpleMath::Vector2(scale.x, scale.y + 40.0f),
		rotation
	);

	AddObject(portal->GetPortal());

	auto clear = std::make_shared<ClearEffect>(DirectX::SimpleMath::Vector2(0.0f), DirectX::SimpleMath::Vector2(scale.x + 430.0f, scale.y + 270.0f), 0.0f);
	playEffect = clear->GetClearEffect()->GetComponent<PlayEffect>("PlayEffect");

	AddObject(clear->GetClearEffect());


	SoundManager::GetInstance().PlayBGM("_Sounds/BGM/FloralLife.wav");
}

void ForestOfTrialsScene::Destroy()
{
	objects.clear();
	worldPlayer = nullptr;
	backGroundImage1 = nullptr;
	backGroundImage2 = nullptr;
	backGroundImage3 = nullptr;
	tileMap = nullptr;
	SoundManager::GetInstance().StopBGM();
}

void ForestOfTrialsScene::Update()
{
	bool isFinished = worldPlayer->GetComponent<PlatformerController>("PlatformerController")->GetIsFinished();

	if (!isCleared)
	ImGuiManager::GetInstance().ShowPlayTime();

	if (!isCleared && isFinished)
	{
		isCleared = true;
		clearTime = TimeManager::GetInstance().GetWorldTime();
		playEffect->Play();
	}
	else if (isCleared)
	{
		ImGuiManager::GetInstance().ShowClearTime(clearTime);

		requestSceneChange = true;
		nextSceneID = SceneID::Title;
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

void ForestOfTrialsScene::DrawBackGroundImage()
{
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
}

void ForestOfTrialsScene::DrawGuideImage()
{
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

	{
		auto guideImage = ObjectFactory::CreateSprite(
			DirectX::SimpleMath::Vector2(180.0f, worldMapHeight - 50.0f),
			DirectX::SimpleMath::Vector2(60.0f, 100.0f), 0.0f,
			L"_Textures/ClearGuide.png"
		);

		AddObject(guideImage);
	}
}

void ForestOfTrialsScene::SpawnMonsters()
{
	{
		auto scarecrow = std::make_shared<Scarecrow>(
			DirectX::SimpleMath::Vector2(tileSize * 13.0f, tileSize * 4.0f),
			DirectX::SimpleMath::Vector2(scale.x - 20.0f),
			rotation
		);

		AddObject(scarecrow->GetMonster());
	}

	{
		auto orangeMushroom = std::make_shared<OrangeMushroom>(
			DirectX::SimpleMath::Vector2(tileSize * 16.0f, tileSize * 9.0f),
			DirectX::SimpleMath::Vector2(scale.x - 40.0f),
			rotation
		);
	
		AddObject(orangeMushroom->GetMonster());
	}

	{
		auto orangeMushroom = std::make_shared<OrangeMushroom>(
			DirectX::SimpleMath::Vector2(tileSize * 2.0f, tileSize * 13.0f),
			DirectX::SimpleMath::Vector2(scale.x - 40.0f),
			rotation
		);

		AddObject(orangeMushroom->GetMonster());
	}

	{
		auto orangeMushroom = std::make_shared<OrangeMushroom>(
			DirectX::SimpleMath::Vector2(tileSize * 24.0f, tileSize * 20.0f),
			DirectX::SimpleMath::Vector2(scale.x - 40.0f),
			rotation
		);

		AddObject(orangeMushroom->GetMonster());
	}

	{
		auto orangeMushroom = std::make_shared<OrangeMushroom>(
			DirectX::SimpleMath::Vector2(tileSize * 2.0f, tileSize * 27.0f),
			DirectX::SimpleMath::Vector2(scale.x - 40.0f),
			rotation
		);

		AddObject(orangeMushroom->GetMonster());
	}

	{
		auto orangeMushroom = std::make_shared<OrangeMushroom>(
			DirectX::SimpleMath::Vector2(tileSize * 27.0f, tileSize * 39.0f),
			DirectX::SimpleMath::Vector2(scale.x - 40.0f),
			rotation
		);

		AddObject(orangeMushroom->GetMonster());
	}

	{
		auto orangeMushroom = std::make_shared<OrangeMushroom>(
			DirectX::SimpleMath::Vector2(tileSize * 22.0f, tileSize * 39.0f),
			DirectX::SimpleMath::Vector2(scale.x - 40.0f),
			rotation
		);

		AddObject(orangeMushroom->GetMonster());
	}

	{
		auto orangeMushroom = std::make_shared<OrangeMushroom>(
			DirectX::SimpleMath::Vector2(tileSize * 17.0f, tileSize * 39.0f),
			DirectX::SimpleMath::Vector2(scale.x - 40.0f),
			rotation
		);

		AddObject(orangeMushroom->GetMonster());
	}

	{
		auto orangeMushroom = std::make_shared<OrangeMushroom>(
			DirectX::SimpleMath::Vector2(tileSize * 12.0f, tileSize * 39.0f),
			DirectX::SimpleMath::Vector2(scale.x - 40.0f),
			rotation
		);

		AddObject(orangeMushroom->GetMonster());
	}
}
