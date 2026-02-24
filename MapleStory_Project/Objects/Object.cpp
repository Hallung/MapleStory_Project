#include "stdafx.h"
#include "Object.h"
#include "Components/Transform.h"

Object::Object(const std::string& name, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, float rotation)
	:name(name)
{
	// Object는 생성 시 기본 Transform을 생성하고 초기 위치/스케일/회전 값을 설정
	transform = std::make_shared <Transform>();

	transform->SetPosition(position);
	transform->SetScale(scale);
	transform->SetRotationDegree(rotation);

	// Transform 또한 하나의 Component이므로 일반 Component와 동일하게 등록하여 관리
	AddComponent(transform);
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
	// Transform은 항상 updateList의 0번에 위치
	// 월드 변환은 모든 Component Update 이후에 수행되어야 하므로 1번 인덱스부터 순회
	for (size_t i = 1; i < updateList.size(); ++i)
		updateList[i]->Update();

	// 마지막에 Transform Update 수행 (최종 위치/회전/스케일 기반 월드 행렬 계산)
	transform->Update();
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