#include "stdafx.h"
#include "TitleScene.h"
#include <cmath>
#include "Utilities/ObjectFactory.h"
#include "Components/MeshRenderer.h"
#include "Resources/Material.h"

void TitleScene::Init()
{
	auto titleImage = ObjectFactory::CreateSprite(
		DirectX::SimpleMath::Vector2(gWinWidth * 0.5f, gWinHeight * 0.5f),
		DirectX::SimpleMath::Vector2(gWinWidth, gWinHeight),
		0.0f,
		L"_Textures/Title.jpg"
	);
	AddObject(titleImage);

	auto logoImage = ObjectFactory::CreateSprite(
		DirectX::SimpleMath::Vector2(gWinWidth * 0.5f, gWinHeight * 0.7f),
		DirectX::SimpleMath::Vector2(700.0f, 370.0f),
		0.0f,
		L"_Textures/Logo.png"
	);
	AddObject(logoImage);

	pressEnterText = ObjectFactory::CreateSprite(
		DirectX::SimpleMath::Vector2(gWinWidth * 0.5f, gWinHeight * 0.3f),
		DirectX::SimpleMath::Vector2(180.0f, 100.0f),
		0.0f,
		L"_Textures/Title2.png"
	);
	pressEnterTextMaterial = pressEnterText->GetComponent<MeshRenderer>("MeshRenderer")->GetMaterial();
	AddObject(pressEnterText);

	SoundManager::GetInstance().PlayBGM("_Sounds/BGM/Title.wav");
}

void TitleScene::Destroy()
{
	objects.clear();
	pressEnterTextMaterial = nullptr;
	pressEnterText = nullptr;
	SoundManager::GetInstance().StopBGM();
}

void TitleScene::Update()
{
	alpha = sinf(static_cast<float>(TimeManager::GetInstance().GetWorldTime()) * 2.0f) * 0.5f + 0.5f;

	pressEnterTextMaterial->SetColor(DirectX::SimpleMath::Color(1.0f, 1.0f, 1.0f, alpha));

	__super::Update();
}

void TitleScene::Render()
{
	__super::Render();
}
