#pragma once
#include "Component.h"
#include "Utilities/CollisionLayer.h"

// Box2D Chain Shape를 생성하여 기울어진 지형을 만드는 컴포넌트
class Chain : public Component
{
public:
	// Chain 컴포넌트 생성(기본 이름: "Chain")
	Chain(b2BodyId bodyId, std::vector<DirectX::SimpleMath::Vector2> points, std::string name = "Chain");
	// Chain 제거 시 Box2D에 생성된 Chain도 함께 제거
	~Chain();

	// 전달받은 점들을 이용하여 Chain Shape 생성
	void CreateChain(b2BodyId bodyId, std::vector<DirectX::SimpleMath::Vector2> points);

	//==================================================================
	// 충돌 레이어 설정
	// 해당 객체가 어떤 Collision Layer에 속하는지 지정
	// 레이어가 변경되면 ApplyFilter()를 호출하여 Shape 필터를 갱신
	//==================================================================
	void SetChainLayer(CollisionLayer layer);

	//===========================================================
	// 충돌 마스크 설정
	// 어떤 레이어들과 충돌할 수 있늕를 비트마스크로 지정
	// 마스크가 변경되면 ApplyFilter()를 호출하여 Shape 필터를 갱신
	//===========================================================
	void SetChainMask(uint32_t mask);

	// 현재 설정된 Layer / Mask 값을 Box2D Shape에 적용하는 함수
	void ApplyFilter() const;

private:
	// Box2D Chain 식별자(체인이 생성되지 않았을 경우 b2_nullChainId 상태)
	b2ChainId chainId = b2_nullChainId;

	// 기본 layer를 Ground로 설정
	CollisionLayer layer = CollisionLayer::Ground;
	// 기본 mask도 최대값으로 설정
	uint32_t mask = 0xFFFFFFFF;
};