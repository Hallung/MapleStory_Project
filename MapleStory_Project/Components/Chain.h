#pragma once
#include "Component.h"

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

private:
	// Box2D Chain 식별자(체인이 생성되지 않았을 경우 b2_nullChainId 상태)
	b2ChainId chainId = b2_nullChainId;
};