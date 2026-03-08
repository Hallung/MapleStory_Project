#include "stdafx.h"
#include "Chain.h"
#include "Utilities/PhysicsUtils.h"

Chain::Chain(b2BodyId bodyId, std::vector<DirectX::SimpleMath::Vector2> points, std::string name)
	: Component(name)
{
	// 초기 상태에서는 Chain이 생성되지 않았으므로 null 상태로 초기화
	chainId = b2_nullChainId;

	// 초기 생성 시 BodyId와 Points를 받아서 생성
	CreateChain(bodyId, points);
}

Chain::~Chain()
{
	// Chain이 유효한 경우 Box2D에서 제거
	// Physics World에 남아있는 Shape 정리
	if (b2Chain_IsValid(chainId))
		b2DestroyChain(chainId);
}

void Chain::CreateChain(b2BodyId bodyId, std::vector<DirectX::SimpleMath::Vector2> points)
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
		chainId = b2_nullChainId;
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

	chainDef.userData = this;

	// Box2D에 Chain 생성
	chainId = b2CreateChain(bodyId, &chainDef);

	// 체인 마찰력 설정 (지형과의 마찰 처리)
	b2Chain_SetFriction(chainId, 1.0);
}

void Chain::SetChainLayer(CollisionLayer layer)
{
	// 동일한 레이어라면 다시 적용할 필요 없음
	if (this->layer == layer) return;

	// 새로운 레이어 설정
	this->layer = layer;
	// 변경된 레이어 정보를 Shape 필터에 적용
	ApplyFilter();
}

void Chain::SetChainMask(uint32_t mask)
{
	// 동일한 마스크라면 다시 적용할 필요 없음
	if (this->mask == mask) return;

	// 충돌 가능한 레이어 마스크 설정
	this->mask = mask;
	// 변경된 마스크 정보를 Shape 필터에 적용
	ApplyFilter();
}

void Chain::ApplyFilter() const
{
	// Chain이 유효하지 않으면 처리하지 않음
	if (b2Chain_IsValid(chainId) == false) return;

	// Chain을 구성하는 segment 개수 가져오기
	UINT segmentCount = b2Chain_GetSegmentCount(chainId);

	// 모든 segment Shape를 저장할 배열
	std::vector<b2ShapeId> segments(segmentCount);

	// 실제 segment Shape들을 가져오기
	UINT returnedCount = b2Chain_GetSegments(chainId, segments.data(), segmentCount);

	// 각 segment Shape에 Collision Filter 적용
	for (int i = 0; i < returnedCount; ++i)
	{
		// 기본 필터 생성
		b2Filter filter = b2DefaultFilter();
		// 해당 객체의 충돌 레이어 설정
		filter.categoryBits = static_cast<uint32_t>(layer);
		// 충돌 가능한 레이어 마스크 설정
		filter.maskBits = mask;
		// groupIndex는 사용하지 않음 (기본값 0)
		filter.groupIndex = 0;

		// Shape에 필터 적용
		b2Shape_SetFilter(segments[i], filter);
	}
}