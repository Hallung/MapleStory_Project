#pragma once
#include "Objects/Object.h"
#include "Components/RigidBody.h"

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

class Player
{
public:
	Player(DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, float rotation, const std::wstring& texturePath = L"_Textures/testPos.png", BodyType bodyType = BodyType::Dynamic, const std::string& name = "Player");
	~Player();

	void Awake();
	void Update();
	void Render();

	enum class State
	{
		HITTING,
		CLIMBING,
		ATTACKING,
		WALKING,
		IDLE,
		NONE
	};

	enum class Ability
	{
		HP, MP, SPEED,
		STR, DEX, INT, LUK,
		NONE
	};

	void SetAbility(Ability ablilty, UINT value);
	AbilityData GetAbility() const { return abilityData; }

	void SetState(State state) { currentState = state; }
	State GetState() const { return currentState; }

	std::shared_ptr<Object> GetPlayer() { return cachPlayer; }

private:
	std::shared_ptr<Object> cachPlayer;
	AbilityData abilityData;
	State currentState = State::NONE;

	BodyType bodyType;
	std::wstring texturePath;
};