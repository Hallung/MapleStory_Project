#include "stdafx.h"
#include "Ground.h"
#include "Components/RigidBody.h"
#include "Components/Chain.h"
#include "Utilities/ObjectFactory.h"

// 전달받은 GroundName에 따라 지형 좌표를 설정하고 지형을 생성
Ground::Ground(GroundName name)
{
	SetGroundPos(name);	// 지형 좌표 설정
	CreateGround(name);	// 실제 Ground Object 생성
}

// 저장된 데이터 초기화
Ground::~Ground()
{
	ground = nullptr;
	groundList.clear();
}

//======================================================
// 지형 좌표 설정 함수
// 해당 GroundName에 대한 좌표가 없다면 새로 생성하여 저장
//======================================================
void Ground::SetGroundPos(GroundName name)
{
	// 이미 해당 지형 좌표가 존재하면 다시 생성하지 않음
	if (groundList.count(name)) return;

	// 화면 중심 좌표
	DirectX::SimpleMath::Vector2 center = { gWinWidth * 0.5f, gWinHeight * 0.5f } ;

	switch (name)
	{
	case Ground::GroundName::SANDBOX:
	{
		// SANDBOX 지형을 구성할 좌표 리스트
		std::vector<DirectX::SimpleMath::Vector2> sendboxPoints;

		// 지형의 각 점(Chain 형태로 연결될 좌표)
		sendboxPoints.push_back({ center.x - 700.0f, center.y - 300.0f });
		sendboxPoints.push_back({ center.x - 600.0f, center.y - 300.0f });
		sendboxPoints.push_back({ center.x - 300.0f, center.y - 250.0f });
		sendboxPoints.push_back({ center.x + 200.0f, center.y - 350.0f });
		sendboxPoints.push_back({ center.x + 600.0f, center.y - 300.0f });
		sendboxPoints.push_back({ center.x + 700.0f, center.y - 300.0f });

		// groundList에 SANDBOX 좌표 저장
		groundList.try_emplace(Ground::GroundName::SANDBOX, move(sendboxPoints));
	}
		break;
	case Ground::GroundName::NONE:
		// 아무 지형도 생성하지 않음
		break;
	}
}

// 특정 지형의 좌표 리스트 반환
const std::vector<DirectX::SimpleMath::Vector2>& Ground::GetGroundPos(GroundName name) const
{
	// 해당 지형 좌표가 존재하면 반환
	if (groundList.count(name))
		return groundList.at(name);

	// 없으면 빈 벡터 반환
	return {};
}

// Ground Object 생성
void Ground::CreateGround(GroundName name)
{
	// 기존 ground가 있다면 제거
	if (ground) ground = nullptr;

	// ObjectFactory를 사용하여 Chain Line 형태의 Object 생성
	ground = ObjectFactory::CreateChainLine(GetGroundPos(name));
	// Static RigidBody 추가 (움직이지 않는 물리 객체)
	ground->AddComponent(std::make_shared<RigidBody>(BodyType::Static));
}

//==========================================
// Chain 컴포넌트 설정
// 실제 물리 엔진에서 체인 형태 충돌체를 생성
//==========================================
void Ground::SetChain(GroundName name)
{
	// Ground Object의 RigidBody 가져오기
	b2BodyId groundId = ground->GetComponent<RigidBody>("RigidBody")->GetBodyId();
	
	//=================================
	// Chain 컴포넌트 추가
	// 지형 좌표를 이용해 물리 체인 생성
	//=================================
	auto groundChain = std::make_shared<Chain>(groundId, GetGroundPos(name));
	
	//======================================================
	// Ground 객체의 Collision Layer를 Ground로 설정
	// 다른 객체들이 Ground 레이어와 충돌 여부를 판단할 때 사용
	//======================================================
	groundChain->SetChainLayer(CollisionLayer::Ground);

	//======================================================
	// Ground가 충돌할 수 있는 레이어 설정
	// Player, Bullet, Monster 레이어와 충돌하도록 Mask 지정
	//======================================================
	groundChain->SetChainMask(
		CollisionLayer::Player | 
		CollisionLayer::Bullet | 
		CollisionLayer::Monster
	);
	
	// 생성한 Chain Collider를 Ground Object에 컴포넌트로 추가
	ground->AddComponent(groundChain);
}
