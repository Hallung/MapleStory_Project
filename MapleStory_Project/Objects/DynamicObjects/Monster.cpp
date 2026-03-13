#include "stdafx.h"
#include "Monster.h"

// Monster 생성자(기본 텍스처 경로와 이름을 설정)
Monster::Monster(const std::wstring& texturePath, BodyType bodyType, const std::string& name)
	: texturePath(texturePath), name(name)
{
}

// Monster 소멸자
Monster::~Monster()
{
}

// 몬스터 능력치 설정
void Monster::SetAbility(Ability ability, UINT value)
{
	switch (ability)
	{
	case Monster::Ability::HP:
		_hp = value;	// 체력 설정
		break;
	case Monster::Ability::SPEED:
		_moveSpeed = value;		// 이동 속도 설정
		break;
	case Monster::Ability::ATACKPOWER:
		_attackPower = value;	// 공격력 설정
		break;
	case Monster::Ability::NONE:
		break;
	}
}

// 몬스터 능력치 조회
UINT Monster::GetAbilityData(Ability ability)
{
	switch (ability)
	{
	case Monster::Ability::HP:
		return _hp;
	case Monster::Ability::SPEED:
		return _moveSpeed;
	case Monster::Ability::ATACKPOWER:
		return _attackPower;
	case Monster::Ability::NONE:
		return -1;	// 잘못된 요청
	}
}

//==================================================================
// 몬스터의 상태를 변경하는 함수
// 동일한 상태로의 중복 변경을 방지하여 불필요한 상태 업데이트를 막는다.
//==================================================================
void Monster::SetState(State state)
{
	// 현재 상태와 동일하면 상태 변경을 하지 않음
	if (currentState == state)
		return;
	// 몬스터 상태 갱신
	currentState = state;
}

//==========================================
// 몬스터가 데미지를 받았을 때 호출되는 함수
// 체력을 감소시키고 상태를 변경
//==========================================
void Monster::TakeDamage(UINT damage)
{
	// 받은 데미지가 현재 체력보다 크거나 같으면 몬스터 사망
	if (damage >= _hp)
	{
		_hp = 0;
		// 몬스터 상태를 사망 상태로 변경
		SetState(State::DIE);
	}
	else
	{
		// 체력 감소
		_hp -= damage;
		// 피격 상태로 변경 (피격 애니메이션 등에 사용)
		SetState(State::HITTING);
	}
}
