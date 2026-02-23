#pragma once
#include "Component.h"

//===================================================
// Box2D 물리 바디 타입을 엔진에서 사용하기 위한 열거형
// Box2D의 b2BodyType 값을 그대로 매핑
//===================================================
enum class BodyType
{
	Static = b2_staticBody,			// 고정 바디(움직이지 않음, 충돌만 함)
	Dynamic = b2_dynamicBody,		// 동적 바디(힘/중력/충돌에 의해 움직임)
	Kinematic = b2_kinematicBody	// 키네마틱 바디(숙도로 움직이며 물리 영향을 받지 않음)
};

//=====================================================================
// Box2D 물리 바디를 엔진 Component 시스템에 통합한 물리 컴포넌트
// Object의 Transform과 동기화되며 물리 시뮬레이션을 통해 이동/충돌을 처리
//=====================================================================
class RigidBody : public Component
{
public:
	// BodyType에 따라 Box2D 바디 종류를 설정
	RigidBody(BodyType type = BodyType::Dynamic, const std::string& name = "RigidBody");
	// 소멸 시 Box2D 월드에서 바디를 제거
	~RigidBody() override;

	// 컴포넌트 초기화 단계, Box2D 바디를 생성/물리 월드에 등록
	void Awake() override;
	// 매 프레임 Box2D 바디 상태를 Transform에 반영
	void Update() override;

	// 속도 설정, 물리 기반 이동
	void SetVelocity(DirectX::SimpleMath::Vector2 velocity);
	// 감쇠 설정, linear: 이동 감쇠, angular: 회전 감쇠
	void SetDamping(float linear, float angular);
	// 회전 고정 여부 설정
	void SetFixedRotation(bool fixed);

	// BodyId 반환
	b2BodyId GetBodyId() const { return bodyId; }

private:
	b2BodyId bodyId = b2_nullBodyId;	// Box2D 바디 식별자
	b2BodyDef bodyDef;					// 바디 생성 정의 정보
};