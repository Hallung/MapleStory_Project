#pragma once
#include "Objects/Object.h"

class RigidBody;

//=====================================================
// 물리 시뮬레이션에 의해 움직이는 오브젝트의 베이스 클래스
// Player, Monster 등에 사용
// 특징:
// - Dynamic RigidBody를 기본으로 가짐
// - 물리 힘, 속도, 충돌 등에 의해 이동 가능
//=====================================================
class DynamicObject : public Object
{
public:
	DynamicObject(const std::string& name, DirectX::SimpleMath::Vector2 position = { 0, 0 }, DirectX::SimpleMath::Vector2 scale = { 1, 1 }, float rotation = 0.0f);

	// DynamicObject가 소유한 RigidBody 반환, 이동이나 점프 등 물리 제어 시 사용
	std::shared_ptr<RigidBody> GetRigidBody() const { return rigidBody; }

private:
	// 물리 시뮬레이션을 담당하는 Dynamic RigidBody
	std::shared_ptr<RigidBody> rigidBody;
};