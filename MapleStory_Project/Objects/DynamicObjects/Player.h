#pragma once
#include "Components/RigidBody.h"

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
	//Player() {}	// 상태나 능력치를 가져올 때 사용할 기본 생성자
	Player(DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, float rotation, const std::wstring& texturePath = L"_Textures/Player1.png", BodyType bodyType = BodyType::Dynamic, const std::string& name = "Player");
	~Player() = default;

	// Player 상태
	enum class State
	{
		HITTING,
		CLIMBING,
		ATTACKING,
		JUMPING,
		STANDING
	};

	// Player 능력치 종류
	enum class Ability
	{
		HP, MP, SPEED,
		STR, DEX, INT, LUK
	};

	//===========================
	// 내부 Object 반환
	// 씬에 등록할 실제 렌더 대상
	//===========================
	std::shared_ptr<Object> GetPlayer() const { return player; }

	// 현재 생성중인 객체의 텍스처 반환
	std::wstring GetCurrentTexture() const { return texturePath; }

	// 객체의 BodyType 반환
	BodyType GetBodyType() const { return bodyType; }

private:
	// Player가 실제로 사용하는 게임 오브젝트
	std::shared_ptr<Object> player;

	// 생성 시 설정값 보관
	BodyType bodyType;
	std::wstring texturePath;
};