#include "stdafx.h"
#include "Chain.h"
#include "Utilities/PhysicsUtils.h"

Chain::Chain(std::string name)
	: Component(name)
{
	// 초기 상태에서는 Chain이 생성되지 않았으므로 null 상태로 초기화
	chainId = b2_nullChainId;
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
		return;

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
		int idx = points.size() - 1 - i;

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
	b2Chain_SetFriction(chainId, 0.5f);
}
