#include "stdafx.h"
#include "BoxCollider.h"
#include "Utilities/PhysicsUtils.h"

//==============================================================================
// Object의 scale 값을 기반으로 Box2D Polygon Shape 생성
// Box2D는 "미터 단위"를 사용하므로, 화면 픽셀 단위를 Physics 월드 단위로 변환해야 함
//==============================================================================
void BoxCollider::CreateShapes(b2BodyId bodyId, const b2ShapeDef& def, DirectX::SimpleMath::Vector2 scale)
{
	//=================================================================
	// Box의 Half Extents 계산
	// Box2D는 전체 크기가 아닌 Half Size를 사용
	// abs(scale) : scale이 음수일 수 있기 때문에 절대값 사용
	// PTM_RATIO : Pixel To Meter Ratio, 화면 좌표 -> 물리 월드 좌표 변환
	//=================================================================
	const float halfScaleX = (abs(scale.x) * 0.5f) / PhysicsUtils::PTM_RATIO;
	const float halfScaleY = (abs(scale.y) * 0.5f) / PhysicsUtils::PTM_RATIO;

	// Collider Offset 적용
	const b2Vec2 center = PhysicsUtils::ScreenToWorld(GetOffset());

	// Box Shape 생성
	const b2Polygon box = b2MakeOffsetBox(halfScaleX, halfScaleY, center, b2MakeRot(0.0f));

	// Box2D Shape 생성 후 shapeIds에 넣기
	shapeIds.push_back(b2CreatePolygonShape(bodyId, &def, &box));
}
