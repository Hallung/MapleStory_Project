#pragma once
#include "Component.h"
#include "Utilities/CollisionLayer.h"

//=================================================================
// Box2D Shape를 기반으로 충돌 판정을 담당하는 Component 베이스 클래스
// 실제 충돌 형태(Box, Circle 등)는 이 클래스를 상속받아 구현한다.
// 주요 역할:
// - Shape 생성 및 제거 관리
// - Transform Scale / Offset 변경 시 Shape 재생성
// - Sensor 여부 설정
// - Collision Exit 이벤트 보조 처리
//=================================================================
class Collider : public Component
{
public:
	Collider();
	virtual ~Collider();

	void Awake() override; // Owner의 Rigidbody를 기반으로 Shape를 생성
	void Update() override; // Transform Scale 변경 감지 시 Shape를 갱신

	// 특정 Point가 Collider 내부에 포함되는지 확인, 각 Collider 타입에서 구현 필요
	virtual bool IsColliding(DirectX::SimpleMath::Vector2 point) = 0;

	// Collider Offset 설정, Offset 변경 시 Shape를 재생성
	void SetOffset(DirectX::SimpleMath::Vector2 val)
	{
		if (offset != val)
		{
			offset = val;
			RefreshShape();
		}
	}

	void SetCollisionLayer(CollisionLayer layer); // 이 Collider가 속할 CollisionLayer 를 설정
	void SetCollisionMask(uint32_t mask); // 이 Collider가 충돌을 허용할 레이어 Mask 설정

	DirectX::SimpleMath::Vector2 GetOffset() const { return offset; } // Offset 반환

	void SetIsSensor(bool value) { isSensor = value; } // Sensor 여부 설정, Sensor는 물리 반응 없이 이벤트만 발생
	bool IsSensor() const { return isSensor; } // Sensor 여부 반환

	b2ShapeId GetShapeId() const { return shapeId; } // Box2D ShapeId 반환

protected:
	// 실제 Shape 생성 함수, 각 Collider 타입에서 구현
	virtual b2ShapeId CreateShapeInternal(b2BodyId bodyId, const b2ShapeDef& def, DirectX::SimpleMath::Vector2 scale) = 0;

	void RefreshShape(); // 현재 Transform 상태를 기반으로 Shape를 재생성

	// Shape 제거 시 Overlap 되어있는 Collider들에게 Collision Exit 이벤트를 전달하기 위한 Callback 함수
	static bool NotifyExitCallback(b2ShapeId otherShapeId, void* context);

	void ApplyFilter() const; // 현재 layer 와 mask 값을 Box2D Shape Filter에 적용

	b2ShapeId shapeId = b2_nullShapeId; // Box2D Shape 식별자
	bool isSensor = false; // Sensor 여부

	DirectX::SimpleMath::Vector2 offset; // Collider Offset

	// 마지막으로 Shape 생성 시 사용한 Scale, Scale 변경 감지용
	// Shape 최초 생성을 보장하기 위해 의도적으로 유효하지 않은 값으로 초기화
	DirectX::SimpleMath::Vector2 lastScale = { -999.0f, -999.0f };

	CollisionLayer layer = CollisionLayer::Default; // 현재 Collider가 속한 Collision Layer
	uint32_t mask = 0xFFFFFFFF; // 충돌 허용 레이어 Mask (0xFFFFFFFF : 모든 레이어와 충돌 허용)
};