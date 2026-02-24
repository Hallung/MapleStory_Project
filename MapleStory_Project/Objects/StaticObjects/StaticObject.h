#pragma once
#include "Objects/Object.h"

//============================================
// 움직이지 않는 정적 오브젝트의 베이스 클래스
// Wall, Ground 등 위치가 고정된 오브젝트에 사용
// 특징:
// - Static RigidBody를 기본으로 가짐
// - 물리 시뮬레이션의 영향을 받지 않음
// - 충돌은 가능하지만 이동하지 않음
// Collider는 추후 구현 후 기본으로 추가 예정
//============================================
class StaticObject : public Object
{
public:
	StaticObject(const std::string& name, DirectX::SimpleMath::Vector2 position = { 0, 0 }, DirectX::SimpleMath::Vector2 scale = { 1, 1 }, float rotation = 0.0f);
};