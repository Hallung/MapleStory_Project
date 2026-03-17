#pragma once
#include "Collider.h"

//===================================================================
// Box 형태의 충돌체를 생성하는 Collider 구현 클래스
// BoxCollider는 Box2D의 Polygon Shape를 사용하여 직사각형 충돌체를 생성
//===================================================================
class BoxCollider : public Collider
{
public:
	//============================================================
	// Box2D Shape 생성 함수
	// bodyId : Shape가 부착될 Box2D Body
	// def    : Shape의 물리 속성 정의 (density, filter, sensor 등)
	// scale  : Object의 현재 크기 (픽셀 단위)
	// 반환값 : 생성된 Box2D ShapeId
	//============================================================
	void CreateShapes(b2BodyId bodyId, const b2ShapeDef& def, DirectX::SimpleMath::Vector2 scale) override;
};