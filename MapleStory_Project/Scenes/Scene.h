#pragma once
#include "stdafx.h"
#include "Objects/Object.h"
#include "Components/Component.h"
#include "Components/Animator.h"

//===========================================
// Scene 베이스 클래스
// 게임의 하나의 화면 단위를 관리하는 클래스
// Object들을 소유하고 Update / Render를 수행
// 각 Scene은 이를 상속받아 Init / Destroy 구현
//===========================================
class Scene
{
public:
	virtual ~Scene() = default;

	virtual void Init() = 0; // Scene 초기화, Object 생성 및 초기 상태 설정
	virtual void Destroy() = 0; // Scene 종료 처리, Scene에서 생성한 리소스 및 Object 정리

	virtual void Update() // Scene에 등록된 모든 Object Update 호출
	{
		PreCleanupDestroyedObjects();
		if (deleteObject)
		{
			if (deleteObject->GetComponent<Animator>("Animator")->IsFinished())
			{
				CleanupDestroyedObjects();
				deleteObject = nullptr;
			}
		}

		for (const auto& obj : objects)
			obj->Update();
	}

	virtual void Render() // Scene에 등록된 모든 Object Render 호출
	{
		for (const auto& obj : objects)
			obj->Render();
	}

	//================================================
	// Object 등록
	// Awake 호출 후 Scene에 추가
	// Awake : Object 초기화 및 Component 초기 설정 수행
	//================================================
	void AddObject(std::shared_ptr<Object> object)
	{
		object->Awake();
		objects.push_back(move(object));
	}

	// 디버그용 ImGui 출력 함수 (필요한 씬에서 override)
	virtual void OnImGui() {}

	void CleanupDestroyedObjects()
	{
		objects.erase(std::remove_if(objects.begin(), objects.end(), [](const std::shared_ptr<Object>& obj)
			{
				return obj->IsDead();
			}), 
			objects.end());
	}

	void PreCleanupDestroyedObjects()
	{
		for (auto& obj : objects)
		{
			if (obj->IsDead())
			{
				obj->OnDestroy();
				deleteObject = obj;
			}
		}
	}

protected:
	//==========================
	// Scene이 소유한 Object 목록
	// Scene 생명주기 동안 관리됨
	//==========================
	std::vector<std::shared_ptr<Object>> objects;

	// TODO: 여러 Object가 사망 시 애니메이션 처리 함수 추후 구현 필요
	std::shared_ptr<Object> deleteObject;
};