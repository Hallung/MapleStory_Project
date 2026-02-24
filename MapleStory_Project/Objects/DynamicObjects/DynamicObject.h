#pragma once
#include "Objects/Object.h"

class RigidBody;

//=======================================================
// 동적 오브젝트의 베이스 클래스
// Player, Monster 등 이동하는 오브젝트에 사용
// 특징:
// - Dynamic RigidBody를 기본으로 생성 (BodyType 변경 가능)
// - 물리 힘, 속도 등에 의해 이동 가능 (Dynamic 기준)
// - Collider는 추후 구현 후 기본으로 추가 예정
//=======================================================
class DynamicObject : public Object
{
public:
	// type 미지정 시 Dynamic으로 생성
	DynamicObject(const std::string& name, 
		DirectX::SimpleMath::Vector2 position = { 0, 0 }, 
		DirectX::SimpleMath::Vector2 scale = { 1, 1 }, 
		float rotation = 0.0f,
		BodyType type = BodyType::Dynamic);

	// DynamicObject가 소유한 RigidBody 반환
	std::shared_ptr<RigidBody> GetRigidBody() const { return rigidBody; }

private:
	// 물리 시뮬레이션을 담당하는 RigidBody (기본값: Dynamic)
	std::shared_ptr<RigidBody> rigidBody;
};