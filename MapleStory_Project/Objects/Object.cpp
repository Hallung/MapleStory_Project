#include "stdafx.h"
#include "Object.h"
#include "Components/Component.h"

Object::Object(const std::string& name, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, float rotation)
	:name(name), position(position), scale(scale), rotation(rotation)
{
	// TODO : Object의 Transform을 어떻게 관리할지 회의 후 리팩토링 예정
}

// Object가 소유한 모든 Component Awake 호출
void Object::Awake()
{
	for (const auto& comp : updateList)
		comp->Awake();
}

// 매 프레임 Component Update 호출
void Object::Update()
{
	for (size_t i = 1; i < updateList.size(); ++i)
		updateList[i]->Update();
}

// Object가 소유한 모든 Component Render 호출
void Object::Render()
{
	for (const auto& comp : updateList)
		comp->Render();
}

//====================================================
// Component 추가
//
// - Component를 components 컨테이너에 등록
// - updateList에도 추가하여 Update / Render 대상에 포함
// - Component의 Owner를 현재 Object로 설정
//
// try_emplace 사용 이유:
// 중복 Component 추가 방지
//====================================================
void Object::AddComponent(const std::shared_ptr<Component>& component)
{
	auto [it, inserted] = components.try_emplace(component->GetName(), component);

	if (inserted)
	{
		updateList.emplace_back(component);
		component->SetOwner(this); // Component의 Owner를 현재 Object로 설정
	}
}