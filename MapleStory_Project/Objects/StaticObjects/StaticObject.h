#pragma once
#include "Objects/Object.h"
#include "Components/RigidBody.h"

//=======================================================
// 정적 오브젝트의 베이스 클래스
// Wall, Ground 등 고정된 오브젝트에 사용
// 특징:
// - Static RigidBody를 기본으로 생성 (BodyType 변경 가능)
// - 물리 시뮬레이션에 의해 이동하지 않음 (Static 기준)
// - 충돌은 가능하지만 자체적으로 이동하지 않음
// - Collider는 추후 구현 후 기본으로 추가 예정
//=======================================================
class StaticObject : public Object
{
public:
	// type 미지정 시 Static으로 생성
	StaticObject(const std::string& name, 
		DirectX::SimpleMath::Vector2 position = { 0, 0 }, 
		DirectX::SimpleMath::Vector2 scale = { 1, 1 }, 
		float rotation = 0.0f,
		BodyType type = BodyType::Static);

	// StaticObject가 소유한 RigidBody 반환
	std::shared_ptr<RigidBody> GetRigidBody() const { return rigidBody; }

private:
	// 물리 시뮬레이션을 담당하는 RigidBody (기본값: Static)
	std::shared_ptr<RigidBody> rigidBody;
};