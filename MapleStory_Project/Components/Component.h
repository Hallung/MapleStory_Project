#pragma once
#include "stdafx.h"

// 모든 컴포넌트의 기본 베이스 클래스
// 게임 오브젝트에 부착되는 기능
// 공통 생명주기(Awake/Update/Render)
class Component
{
public:
	// 컴포넌트 이름 생성 (복사)
	Component(const std::string& componentName) : name(componentName) {}
	// 컴포넌트 이름 생성 (이동)
	Component(std::string&& componentName) : name(move(componentName)) {}
	virtual ~Component() = default;

	// 컴포넌트 초기화 단계
	// 생성 시 호출
	virtual void Awake() {}
	
	// 프레임 업데이트
	// 매 프레임 호출
	// 로직 처리 및 상태 갱신
	virtual void Update() {}

	// 렌더 단계
	// 렌더 프레임마다 호출
	virtual void Render() {}

	// 컴포넌트 이름 반환
	std::string GetName() const { return name; }

protected:
	std::string name;	// 컴포넌트 식별 이름
};