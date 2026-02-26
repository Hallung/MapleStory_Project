#pragma once
#include "Objects/Object.h"
#include "Components/RigidBody.h"

//=========================================
// 플레이어 능력치 데이터
// 기본 스탯 값을 보관하는 구조체
// 게임 진행 중 Player가 변경/성장 시키는 값
//=========================================
struct AbilityData
{
	UINT _hp = 100;
	UINT _mp = 100;
	UINT _moveSpeed = 200;
	UINT _str = 4;
	UINT _dex = 4;
	UINT _int = 4;
	UINT _luk = 4;
};

//=========================================
// Player 관리 클래스
// 실제 게임 오브젝트를 내부에 생성/보관
// 능력치/상태/로직을 관리하는 컨트롤러 역할
// 렌더/물리/컴포넌트는 Object가 담당
//=========================================
class Player
{
public:
	//==================================================
	// Player 생성
	// Sprite Object를 생성하고 Player 구성 컴포넌트 부착
	//==================================================
	Player(DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, float rotation, const std::wstring& texturePath = L"_Textures/testPos.png", BodyType bodyType = BodyType::Dynamic, const std::string& name = "Player");
	~Player();

	// Player 상태
	enum class State
	{
		HITTING,
		CLIMBING,
		ATTACKING,
		WALKING,
		IDLE,
		NONE
	};

	// Player 능력치 종류
	enum class Ability
	{
		HP, MP, SPEED,
		STR, DEX, INT, LUK,
		NONE
	};

	//=====================================
	// 능력치 증가
	// Ability 종류에 따라 해당 스탯 값 누적
	//=====================================
	void SetAbility(Ability ablilty, UINT value);
	// 현재 능력치 반환
	AbilityData GetAbility() const { return abilityData; }

	// 상태 설정 / 조회
	void SetState(State state) { currentState = state; }
	State GetState() const { return currentState; }

	//===========================
	// 내부 Object 반환
	// 씬에 등록할 실제 렌더 대상
	//===========================
	std::shared_ptr<Object> GetPlayer() const { return cachPlayer; }

	// 현재 생성중인 객체의 텍스처 반환
	std::wstring GetCurrentTexture() const { return texturePath; }

	// 객체의 BodyType 반환
	BodyType GetBodyType() const { return bodyType; }

private:
	// Player가 실제로 사용하는 게임 오브젝트
	std::shared_ptr<Object> cachPlayer;

	//플레이어 능력치 데이터
	AbilityData abilityData;

	// 현재 상태
	State currentState = State::NONE;

	// 생성 시 설정값 보관
	BodyType bodyType;
	std::wstring texturePath;
};