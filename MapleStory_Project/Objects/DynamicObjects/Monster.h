#pragma once
#include "Objects/Object.h"
#include "Components/RigidBody.h"

//================================================
// 몬스터의 기본 데이터를 관리하는 베이스 클래스
// 모든 몬스터 타입은 이 클래스를 기반으로 확장
//================================================
class Monster
{
public:
	//=================================================
	// 생성자
	// texturePath : 몬스터 기본 텍스처 경로
	// bodyType : 물리 바디 타입 (Dynamic / Static 등)
	// name : 몬스터 이름
	//=================================================
	Monster(
		const std::wstring& texturePath = L"_Textures/testPos.png", 
		BodyType bodyType = BodyType::Dynamic, 
		const std::string& name = "Monster");
	// 소멸자
	~Monster();

	// 몬스터 상태 정의
	enum class State
	{
		HITTING,
		ATTACKING,
		JUMPING,
		STANDING,
		MOVE,
		DIE,
		NONE
	};

	// 몬스터 능력치 종류 정의
	enum class Ability
	{
		HP,
		SPEED,
		ATACKPOWER,
		NONE
	};

	// 능력치 설정 함수
	void SetAbility(Ability ability, UINT value);
	// 능력치 조회 함수
	UINT GetAbilityData(Ability ability);

	// 몬스터 Object 반환
	std::shared_ptr<Object> GetMonster() const { return monster; }

	// 몬스터 상태를 설정하는 함수(state : 변경할 몬스터 상태)
	void SetState(State state);
	// 현재 몬스터 상태를 반환하는 함수
	State GetState() const { return currentState; }

	// 몬스터가 데미지를 받았을 때 호출되는 함수(damage : 플레이어 공격 등으로 받은 데미지 값)
	void TakeDamage(UINT damage);

	virtual void OnStateChanged(State newState) {}

protected:
	std::shared_ptr<Object> monster;	// 실제 씬에 존재하는 몬스터 Object
	std::wstring texturePath;				// 몬스터 텍스처 경로
	std::string name;						// 몬스터 이름
	State currentState = State::NONE;		// 몬스터 상태

private:
	UINT _hp = 100;			// 체력
	UINT _moveSpeed = 200;	// 이동 속도
	UINT _attackPower = 0;	// 공격력
};