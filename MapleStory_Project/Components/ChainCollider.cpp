#include "stdafx.h"
#include "ChainCollider.h"
#include "Utilities/PhysicsUtils.h"

ChainCollider::ChainCollider(std::vector<DirectX::SimpleMath::Vector2> points, std::string name)
	: Collider(name), points(points)
{
}

ChainCollider::~ChainCollider()
{
	// Chain이 유효한 경우 Box2D에서 제거
	// Physics World에 남아있는 Shape 정리
	if (b2Chain_IsValid(chainId))
		b2DestroyChain(chainId);
}

void ChainCollider::CreateShapes(b2BodyId bodyId, const b2ShapeDef& def, DirectX::SimpleMath::Vector2 scale)
{
	// Body가 유효하지 않으면 Chain 생성 불가
	if (!b2Body_IsValid(bodyId))
	{
		return;
	}

	// 이미 Chain이 존재한다면 기존 Chain 제거 후 재생성
	if (b2Chain_IsValid(chainId))
	{
		b2DestroyChain(chainId);
	}

	// 기본 Chain 정의 생성
	b2ChainDef chainDef = b2DefaultChainDef();

	// Body의 현재 월드 위치 가져오기
	b2Vec2 bodyPos = b2Body_GetPosition(bodyId);

	// 화면 좌표 -> 물리 월드 좌표로 변환된 포인트 목록
	std::vector<b2Vec2> worldPoints;

	//=============================================================
	// 입력된 화면 좌표를 Box2D 월드 좌표로 변환
	// Box2D는 Body 기준 로컬 좌표를 사용하므로 Body 위치를 빼서 저장
	//=============================================================
	for (size_t i = 0; i < points.size(); ++i)
	{
		size_t idx = points.size() - 1 - i;

		// Screen 좌표 -> Box2D World 좌표 변환
		b2Vec2 world = PhysicsUtils::ScreenToWorld(points[idx]);

		// Body 기준 로컬 좌표로 변환
		worldPoints.push_back(world - bodyPos);
	}

	// Chain에 사용할 포인터 배열 설정
	chainDef.points = worldPoints.data();
	// 포인트 개수 설정
	chainDef.count = static_cast<int>(points.size());
	// 루프 여부(false = 열린 체인, true = 닫힌 체인)
	chainDef.isLoop = false;

	// Box2D에 Chain 생성
	chainId = b2CreateChain(bodyId, &chainDef);

	// 체인 마찰력 설정 (지형과의 마찰 처리)
	b2Chain_SetFriction(chainId, 1.0);

	// Chain Segment 개수 가져오기
	UINT count = b2Chain_GetSegmentCount(chainId);

	// Segment 개수만큼 백터 공간 마련
	std::vector<b2ShapeId> segments(count);

	// 마련해둔 백터 공간에 Segment 정보 담기
	UINT returned = b2Chain_GetSegments(chainId, segments.data(), count);

	// 각 segment에 shapeId를 설정
	for (UINT i = 0; i < returned; ++i)
	{
		b2ShapeId shapeId = segments[i];

		b2Shape_SetUserData(shapeId, this);
		// shapeId를 저장
		shapeIds.push_back(shapeId);
	}

	// 필터 적용
	ApplyFilter();
}