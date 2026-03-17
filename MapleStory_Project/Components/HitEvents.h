#pragma once
#include "Component.h"
#include "Utilities/CollisionLayer.h"
#include <unordered_set>


//====================================================
// HitEvents 컴포넌트
// 현재 객체가 충돌 중인 Collider들을 관리하는 컴포넌트.
// OnCollisionEnter / OnCollisionExit 이벤트를 통해
// 현재 충돌 중인 Collider 목록을 유지한다.
// 이후 특정 CollisionLayer와 충돌 중인지 여부를
// IsCollidingWith() 함수를 통해 쉽게 확인할 수 있다.
//====================================================
class HitEvents : public Component
{
public:
	// 기본 생성자(Component 이름을 "HitEvents"로 설정)
	HitEvents() : Component("HitEvents"){}
	// 컴포넌트 초기화 시 호출
	void Awake() override;
	// 매 프레임 호출되는 업데이트 함수
	void Update() override;
	// 충돌이 시작될 때 호출되는 이벤트
	void OnCollisionEnter(Collider* other) override;
	// 충돌이 종료될 때 호출되는 이벤트
	void OnCollisionExit(Collider* other) override;
	// 특정 CollisionLayer와 현재 충돌 중인지 확인하는 함수
	bool IsCollidingWith(CollisionLayer layer) const;

private:
	// 현재 충돌 중인 Collider들을 저장하는 컨테이너
	// unordered_set을 사용하여
	// - 중복 저장 방지
	// - 빠른 삽입 / 삭제
	std::unordered_set<Collider*> currentColliders;
};