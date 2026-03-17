#pragma once
#include "Objects/Object.h"

// 지형(Ground)을 생성하고 관리하는 클래스
class Ground
{
public:
	// 생성할 지형 종류
	enum class GroundName
	{
		SANDBOX,	// 샌드박스 현태의 지형
		NONE		// 지형 없음
	};

	// 생성자(지형 이름을받아 해당지형 데이터를 설정하고 생성)
	Ground(GroundName name);
	// 소멸자(Ground 소멸 시 저장 데이터 초기화)
	~Ground();

	// 지형 좌표 설정(특정 지형 이름에 맞는 좌표 데이터를 생성하여 저장)
	void SetGroundPos(GroundName name);
	// 지형 좌표 반환(해당 지형의 좌표 리스트를 반환)
	const std::vector<DirectX::SimpleMath::Vector2>& GetGroundPos(GroundName name) const;

	// 실제 지형 Object 생성
	void CreateGround(GroundName name);
	// 생성된 Ground Object 반환
	std::shared_ptr<Object> GetGround(GroundName name) const { return ground; }
	// Chain(체인 충돌) 컴포넌트 설정
	void SetChain(GroundName name);

private:
	// 실제 게임에 생성되는 Ground Object
	std::shared_ptr<Object> ground;

	// 지형 이름별 좌표 리스트 저장
	std::unordered_map<GroundName, std::vector<DirectX::SimpleMath::Vector2>> groundList;
};