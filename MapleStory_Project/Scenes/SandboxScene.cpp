#include "stdafx.h"
#include "SandboxScene.h"

// Scene 초기화, 테스트용 Object AddObject()로 등록 가능
void SandboxScene::Init()
{

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
}

// Scene의 기본 Object Render 수행, 필요 시 디버그 렌더링 등 확장 가능
void SandboxScene::Render()
{
	__super::Render();
}
